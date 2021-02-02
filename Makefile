# =================================================================================
# ================================= CONFIGURATION =================================
# =================================================================================

include config

EXEC := ${BINDIR}/PolyShell
SRCFILES := $(shell find ${SRCDIR} -name '*.c')
TESFILES := $(shell find ${TESDIR} -name '*.c')

# =================================================================================
# ===================================== BUILD =====================================
# =================================================================================

build: ${EXEC}
-include $(shell find ${OBJDIR} -name        '*.d')
-include $(shell find     .     -name 'Makefile.*')

%.o:
	$(shell mkdir -p $(shell dirname $@))
	${CC} -o $@ -c $(@:${OBJDIR}/%.o=%.c) ${IFLAGS} ${CFLAGS}

${EXEC}: $(SRCFILES:%.c=${OBJDIR}/%.o) $(TESFILES:%.c=${OBJDIR}/%.o)
	${CC} -o $@ $^ ${LFLAGS}

clean:
	rm -rf ${OBJDIR}/*

mrproper: clean
	rm -rf ${EXEC}

rebuild: mrproper build

# =================================================================================
# ============================== UPDATE/SAVE/RESTORE ==============================
# =================================================================================

.PHONY: save restore update submit-c submit-unix clear-infos

submit-c:
	./scripts/submit.sh "projet-c"

submit-unix:
	./scripts/submit.sh "unix"

save:
	./scripts/save.sh

restore:
	./scripts/restore.sh

clear-infos:
	./scripts/clear-infos.sh

update:
	./update.sh

# =================================================================================
# ================================= TEST/VALGRIND =================================
# =================================================================================

.PHONY: run valgrind

run: ${EXEC}
	@ ${EXEC} ${RFLAGS} ${ARGS}

valgrind: ${EXEC}
	@ G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --track-origins=yes ${VFLAGS} ${EXEC} --unit-tests --unsecured-tests --command=clear_exit ${ARGS}

test: ${EXEC}
	@ rm -rf /tmp/polyshell.tests/ /tmp/polyshell.tests.bash/ /tmp/polyshell.tests.poly/
	@ mkdir -p /tmp/polyshell.tests/
	@ sh ./scripts/test.sh > /tmp/polyshell.tests/main.txt 2>&1                         # bash
	@ cp -R /tmp/polyshell.tests/ /tmp/polyshell.tests.bash/                            # bash
	@ ${EXEC} --quiet ${ARGS} -f ./scripts/test.sh > /tmp/polyshell.tests/main.txt 2>&1 # polyshell
	@ mv /tmp/polyshell.tests/ /tmp/polyshell.tests.poly/                               # polyshell
	@ diff -rq /tmp/polyshell.tests.bash/ /tmp/polyshell.tests.poly/ > /dev/null 2>&1 \
	                           && { echo "Passed! Congratulations!"; exit 0; } \
	                           || { echo "Failed! Look at the differences between /tmp/polyshell.tests.bash/ and /tmp/polyshell.tests.poly/."; exit ${ERRSTATUS}; }

# =================================================================================
# =============================== MISCELLANEOUS ===================================
# =================================================================================

.PHONY: website

website:
	@ ./scripts/website.sh

# =================================================================================
# =================================================================================
# =================================================================================
