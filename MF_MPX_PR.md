

## Class MFMultiplexer

- _Purpose_: manage 4 selection lines for external multiplexers, on any Arduino pins (defined as type `kTypeMultiplexer`).  
  These lines will be connected in parallel to every hardware multiplexer used (typically 74HCT4067 or -4051).
- One instance only (variable `MPX`), activated with following command:
 
    `kTypeMultiplexer.Sel0_pin.Sel1_pin.Sel2_pin.Sel3_pin`

- The multiplexer can be used alone (to route inputs or outputs of other blocks such as display drivers, shift registers etc), or as required component for other blocks (e.g. direct multiplexed digital / analog inputs).  
  - In the first case, I/O blocks will exploit the fact that the MPX automatically increments the selector value at every iteration of the main loop;  
  blocks can read the current selector value and determine their behavior accordingly.
  - In the second case, blocks can drive the selector as they require (e.g. doing a complete scan) with no interference with the main loop iteration.

## Class MFMPXDigitalIn

- _Purpose_: manage 8/16 digital input lines, multiplexed on a single Arduino pins (defined as type `kTypeMPXDigitalIn`). These lines will be connected to a hardware multiplexer (typically 74HCT4067 or -4051), whose selector lines will be connected to the pins defined by the MFMultiplexer component defined separately.
- Several instances (see  `MAX_DIG_IN_MPX` in files `MFBoards.h`), activated with following command:  

    `kTypeMPXDigitalIn.dataPin.halfSize.mode.name`  

  where:  
  - `dataPin` is the common Arduino input pin
  - `halfSize` is `0` for 16-channel MPXs (eg 4067) or `1` for 8-channel MPXs (eg 4051)
  - `mode` is `0` if the block should perform a full scan whenever polled, or `1` if it should read on bit only for each polling cycle (following the value of the main loop MPX count).  
  In both cases, changed inputs immediately generate event callbacks (of type `kMPXDigitalInChange`).
