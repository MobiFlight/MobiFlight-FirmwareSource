# Unreleased

* Fixed incorrect count of bytes shifted out. It did not affect operation because the excess byte was on the tail end and was discarded, however it unnecessarily increased timing and might cause problems in the future.
* Migrated to PlatformIO for builds