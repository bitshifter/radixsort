RUSTC=~/opt/rust/bin/rustc
RFLAGS=-g --opt-level=3
.PHONY all: perf test
perf: perf.rs radix.rs helpers.rs
	${RUSTC} ${RFLAGS} perf.rs
test: test.rs radix.rs helpers.rs
	${RUSTC} ${RFLAGS} test.rs
