# msp430ref

Reference firmware for the MSP430G2553 (LaunchPad / G2 family): 
  - hardware PWM via Timer_A 
  - ADC10 in sequence mode. 
  
  ## Pin map:


![MSP430G2x53 pinout](./img/msp430G2x53_pins.png)

## Aim of the firmware

The firmware provides a clean API for:

- **Hardware PWM channels** — `pwm.h`: `PWM_Configure(port, period)` once, then `PWM_SetDuty(port, duty_Q8)` at runtime. `SetPWMOut(...)` is kept as a one-shot wrapper. Supported `PWMPorts`: `P1_2`, `P1_6`, `P2_1`, `P2_2`, `P2_4`, `P2_5`, `P2_6`.
- **ADC inputs** — `adc.h`: `ConfigureADC(Ax)` can be called per channel; the driver accumulates the set and reprograms the ADC10 sequencer accordingly. `GetADCValue(Ax)` triggers a fresh sequence, waits for completion, and returns the 10-bit sample.
- **Flash-backed settings** — `flash.h`: `Flash_Init`, `Flash_EraseSegment`, `Flash_Write`, `Flash_Read` against INFOB/C/D (INFOA is left alone because it holds the DCO calibration). `main.c` uses `FLASH_INFOD` to persist PWM duties and period across resets, with a magic-word check to initialise defaults on first boot.



## Toolchain and build

The `Makefile` uses Texas Instruments MSP430 GCC (`msp430-elf-gcc`), not the older `msp430-gcc` from the Debian/Ubuntu `gcc-msp430` package. Adjust these variables to match your install:

| Variable | Purpose |
|----------|---------|
| `MSPGCC_ROOT_DIR` | Root of the TI `msp430-gcc` package (default in repo: `/Users/emil/ti/msp430-gcc`) |
| `MSPGCC_BIN_DIR` | Usually `$(MSPGCC_ROOT_DIR)/bin` |

Build artifacts go under `obj/` and `bin/`.

```bash
make all    # produces bin/main.elf
make clean
make debug  # runs mspdebug (see below) with bin/main.elf
```

The MCU is set in the Makefile as `msp430g2553` (`-mmcu=msp430g2553`).

## Flash and debug

Programming uses [mspdebug](https://dlbeer.co.nz/mspdebug/) with the `rf2500` driver (typical for the MSP-EXP430G2 LaunchPad’s on-board debugger). After `make all`:

```bash
./flash.sh
```

`flash.sh` programs `bin/main.elf`. The Makefile `debug` target also invokes `mspdebug rf2500` with that ELF.

## Application notes

- Add appropriate analog front-end (e.g. input filtering or a capacitor per channel) for stable ADC readings, per your hardware and the device datasheet.
- For smoother or load-friendly PWM, consider RC filtering or buffering on PWM outputs, depending on what you drive.

## Local documentation

PDFs under `docs/` (e.g. device and user guide) are included for offline reference.

## Demo: analog neural network across several MSP430G2553s

The three APIs above (PWM, ADC, flash) are enough to build a small analog neural-network demonstrator where each chip represents one neuron: the neuron's activation is shown on an LED, travels to downstream neurons as a smoothed PWM voltage, gets summed at a junction of resistors, and the downstream chip applies an activation function in software before driving its own PWM.

### Mapping neural-network concepts to the hardware

| NN concept                   | Physical element |
|------------------------------|------------------|
| Neuron's activation value    | PWM duty on one MSP430's output pin |
| "Visible" activation         | LED on that pin, brightness = activation |
| Axon / wire                  | Physical wire from a PWM pin toward the next neuron |
| Synaptic weight `wᵢ`         | Series resistor `Rᵢ` on that wire |
| Dendritic summation `Σwᵢxᵢ`  | Node where those resistors meet |
| Soma + activation `f(·)`     | Receiving chip's ADC reading + software |
| Neuron output                | New PWM duty on the receiving chip |

With `PWMPorts` giving 3 independent HW PWM outputs per chip and `ADC_Channel` giving up to 6 ADC inputs, one G2553 can act as a neuron with up to 3 fan-out edges and ~4 useful fan-in edges. A 2-2-1 network is comfortable on five G2553 boards.

### Signal path per "neuron" output

Each PWM pin drives two parallel branches off the same pad — one for visibility, one for the data path:

```
PWM_OUT ──┬── [ R_led ]── LED ── GND             (visible activation)
          │
          └── [ R_filt ]──┬── V_smoothed ─── toward downstream summing node
                          │
                        [ C ] ── GND             (low-pass to DC)
```

With `period = 255` at 1 MHz SMCLK the PWM is ~490 Hz, so `R_filt = 10 kΩ`, `C = 1 µF` (`f_c ≈ 16 Hz`) gives clean DC with ~50–100 ms settling. Drop `period` to speed the network up.

### Passive weighted summing at an input

The cheapest "dendrite": just tie each incoming wire to the ADC pin through its own weight resistor `R_wᵢ`.

```
V_smoothed_1 ──[ R_w1 ]──┐
V_smoothed_2 ──[ R_w2 ]──┼──> ADC_IN on receiving chip
V_smoothed_3 ──[ R_w3 ]──┘
                         │
                       [ C_in ] ── GND   (optional extra smoothing)
```

Node voltage:

`V_node ≈ Σ(Vᵢ / R_wᵢ) / Σ(1 / R_wᵢ)`

Smaller `R_wᵢ` = bigger weight. With equal resistors it degenerates to the plain average and the receiving chip multiplies by `N` in software to recover the sum. Weights are positive-only; for signed weights use the op-amp summer described further down.

### Op-amp summing (when you want true `Σ`, gain, and signed weights)

```
              R_f
          ┌──/\/\──┐
          │        │
V₁──[R₁]──┤        │
V₂──[R₂]──┤── (−)──┼── V_out  = −R_f · ( V₁/R₁ + V₂/R₂ + … )
V₃──[R₃]──┤   OA   │
          │── (+)──┤
                V_ref = Vcc/2
```

- `wᵢ = R_f / Rᵢ` sets per-synapse gain; `R_f` sets the neuron's overall gain.
- Single-supply: bias the `+` input to `Vcc/2` and interpret the ADC reading relative to mid-rail so the output can "swing negative" numerically.
- Rail-to-rail single-supply parts such as **MCP6002** (dual) or **MCP6004** (quad) work fine at 3.3 V.
- Signed weights: feed the input through an inverter stage before the summer to implement inhibitory synapses; a single `R_bias` from `V_ref` into the summer implements the neuron's bias term.

### Practical notes

- Share **common ground and common Vcc** across every chip and the summing nodes. The ADC driver uses `AVcc` as the reference by default, so mismatched supplies distort readings.
- Keep the filter resistor `R_filt` low enough (≲ few kΩ) or buffer with an op-amp follower if the downstream ADC's source-impedance requirement is tight — ADC10 wants low source impedance during sampling.
- For software-adjustable weights, replace each `R_wᵢ` with a digital potentiometer (e.g. **MCP4131** over SPI) driven by a host chip.
- Store weights/biases/LUTs (ReLU threshold, sigmoid table) in INFO flash via `flash.h` so the network's personality survives resets.

### Sketch: step 3 — two inputs summing into one "neuron" chip

Three chips:

- **MCU_A, MCU_B** — input neurons. Each drives one PWM pin with a user-chosen duty; each has an LED on that pin and a filtered branch feeding a weight resistor.
- **MCU_C** — receiving neuron. Reads the summed voltage on one ADC pin, applies a threshold/activation in software, drives its own PWM (and LED) with the result.

Wiring:

```
MCU_A: P2.1 ──┬── [330Ω]── LED_A ── GND
              └── [10kΩ]──┬────────[R_wA = 10k]──┐
                          │                       │
                        [1µF] ── GND              │
                                                  ├── MCU_C: ADC input A0 (P1.0)
MCU_B: P2.1 ──┬── [330Ω]── LED_B ── GND           │
              └── [10kΩ]──┬────────[R_wB = 10k]──┘
                          │
                        [1µF] ── GND

MCU_C: P2.6 ──── [330Ω]── LED_C ── GND     (this neuron's output activation)

All three chips share GND and 3.3V.
```

Firmware per chip, using the APIs already in this repo:

**`main.c` on MCU_A and MCU_B** (input neurons)

```c
#include "pwm.h"
#include "flash.h"

int main(void) {
    init_clock_and_wdt();        // DCO 1 MHz, WDT off
    Flash_Init();

    settings_t s;                 // { magic, duty, period, ... }
    load_or_init_settings(&s);    // same pattern as this repo's main.c

    PWM_Configure(P2_1, s.period);
    PWM_SetDuty  (P2_1, s.duty); // e.g. 0x40 for "low", 0xC0 for "high"

    for (;;) { /* optionally vary duty over time, persist on change */ }
}
```

**`main.c` on MCU_C** (receiving neuron — the "soma")

```c
#include "pwm.h"
#include "adc.h"
#include "flash.h"

#define THRESHOLD_COUNTS  400u   // ~1.3 V at Vcc=3.3V, tuned empirically
#define AVG_TO_SUM_GAIN   2u     // two passive inputs -> recover sum

int main(void) {
    init_clock_and_wdt();
    Flash_Init();

    ConfigureADC(A0);            // summed dendrite input on P1.0
    PWM_Configure(P2_6, 255);    // output neuron on P2.6 / TA0.1

    for (;;) {
        uint16_t v_avg = GetADCValue(A0);        // 0..1023 avg of the two inputs
        uint16_t v_sum = v_avg * AVG_TO_SUM_GAIN; // recover sum from passive averager

        uint8_t y;
        if (v_sum <= THRESHOLD_COUNTS) {
            y = 0;                                // below threshold: silent
        } else {
            uint16_t over = v_sum - THRESHOLD_COUNTS;
            y = (over > 0xFF) ? 0xFF : (uint8_t)over;  // ReLU, clamped to 8-bit
        }

        PWM_SetDuty(P2_6, y);                     // drives LED_C + downstream
    }
}
```

What to watch on the bench:

1. Power all three boards from a shared 3.3 V rail; verify `LED_A`, `LED_B`, `LED_C` off at reset.
2. Raise `MCU_A`'s duty; `LED_A` brightens, `LED_C` stays dark as long as the sum is below threshold.
3. Raise `MCU_B`'s duty; when the summed voltage crosses `THRESHOLD_COUNTS`, `LED_C` lights — that's the neuron firing.
4. Swap `R_wA` for a smaller value (say 3.3 kΩ) and MCU_A now dominates the sum: a weight change you can see.
5. Store `THRESHOLD_COUNTS` and the per-neuron duty/period in INFO flash (`Flash_EraseSegment` + `Flash_Write`) so the network survives a power-cycle.

Scaling up: replace the passive summer with an MCP6004 summing amp (above) to get true addition, bias, and signed weights; chain MCU_C's PWM into the next layer's summing node the same way.

