Version 1.0.1-dev
-----------------
- Preliminary support for a subset of the Vector Extension, v0.7.1.
- Support S-mode vectored interrupts (i.e. `stvec[0]` is now writable).
- Several debug-related additions and changes:
  - Added `hasel` debug feature.
  - Added `--dm-no-abstract-csr` command-line option.
  - Added `--dm-no-halt-groups` command line option.
  - Renamed `--progsize` to `--dm-progsize`.
  - Renamed `--debug-sba` to `--dm-sba`.
  - Renamed `--debug-auth` to `--dm-auth`.
  - Renamed `--abstract-rti` to `--dm-abstract-rti`.
  - Renamed `--without-hasel` to `--dm-no-hasel`.

Version 1.0.0 (2019-03-30)
--------------------------
- First versioned release.
