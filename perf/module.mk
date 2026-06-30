perf/%: CC := gcc
perf/%: CFLAGS += -std=c23
perf/%: CPPFLAGS += -I.


perf/lc_clock_cmp/perf.o: perf/lc_clock_cmp/lhs.rand.dat perf/lc_clock_cmp/rhs.rand.dat

DEP += perf/lc_clock_cmp/perf.d

perf/lc_clock_cmp/lhs.rand.dat: perf/lc_clock_cmp/gen_data
	$^ $@
perf/lc_clock_cmp/rhs.rand.dat: perf/lc_clock_cmp/gen_data
	$^ $@

perf/%.rand.dat:
	dd bs=4M count=1 if=/dev/random of=$@
