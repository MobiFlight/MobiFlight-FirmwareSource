# Unreleased

* Event handlers for MFButton unified, also for MFEncoder (all transitions use the same handler anyway)
* Event handlers for MFButton, MFEncoder and MFAnalog (pointers and assignment functions) made static
  (no need to define a different handler for each button. Exception: Stepper zero, see other remark).
* MFAnalog: removed handler assignment from constructor; introduced separate attachHandler() function like MFButton/MFEncoder
* MFStepper: removed dependency from MFButton in order to avoid the need for separate handler
* MFButton: added status argument to trigger() to avoid unnecessary double call
* Fixed incorrect count of bytes shifted out. It did not affect operation because the excess byte was on the tail end and was discarded, however it unnecessarily increased timing and might cause problems in the future.
* Migrated to PlatformIO for builds