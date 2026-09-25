# Linux regression tests

Use GCC 14, matching gcov 14, CMake, Ninja and gcovr 8.6. The CI installer
supports Ubuntu 24.04. From the repository root:

```sh
export CC=gcc-14 GCOV=gcov-14 TX_COVERAGE=ON
./scripts/build.sh
./scripts/test.sh
```

The runner verifies the dependency commits recorded in `threadx-revision.txt`
and `filex-revision.txt`. The FileX revision is the tested head of
[eclipse-threadx/filex#106](https://github.com/eclipse-threadx/filex/pull/106),
which is still open. Updating a dependency requires changing its pin and
rerunning all ten configurations. An existing checkout with another revision
or modified tracked files is rejected.

All configurations need the FileX source tree. Non-standalone configurations
link the shared ThreadX and FileX libraries; standalone configurations build
FileX with standalone support. LevelX has no Windows regression port.

`TX_COVERAGE=ON` instruments every LevelX configuration. A complete test run
clears old coverage first, runs every configuration, and collects coverage even
when a test fails. Reports under `coverage_report/per_configuration` contain
JSON, XML and HTML for each configuration. The `merged` reports combine all ten
JSON inputs and use repository-relative source names. Missing, empty or
unmeasured inputs fail collection. Both line and branch floors are enforced by
`coverage.sh`; the workflow also enforces its integer line floor.

After a successful full run, `./test/cmake/check_coverage.sh` verifies rejection
of missing, empty and unmeasured coverage inputs, then restores and remerges the
valid reports.

The reusable workflow pin supplies bounded install, build and test steps and
retains test and coverage artifacts on failure. Only master push or manual
runs can deploy coverage. Dependabot updates target dev; GitHub activates this
configuration once it is present on the default branch.

## Coverage target

The measured GCC 14 union is 1,632/2,400 lines (68.00%) and 1,596/2,439
branches (65.44%). The enforced floors are 68.0% lines and 65.4% branches.
The 100% target still needs 768 lines and 843 branches covered. The largest
line gaps are the FileX simulator adapters (164), NOR block reclaim (69), NOR
extended open (65), and NAND block data movement (38). Other gaps include
media-error paths, metadata allocation, sector release and simulator failures.
These sources remain in the denominator.

The ECC regression checks every single-bit position in a 512-byte page,
corrections in both halves, and uncorrectable errors. All four ECC helper files
have full line coverage. The full, driver-interface and combined NOR cache
configurations contribute 97, 51 and 65 source lines absent from the default
configuration respectively. Standalone configurations select a subset of the
same source lines; their reports still participate in the union. Function
merging uses the earliest declaration line because driver-interface macros
place otherwise identical function declarations on different lines.

The ECC test views aligned `USHORT` storage through a character pointer because
the ECC implementation accesses words. This is a deviation from advisory
MISRA C:2004 Rule 11.4; character access preserves alignment and is explicitly
permitted by the character-pointer exception in MISRA C:2012/2023 Rule 11.3.
