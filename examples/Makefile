
override CPPFLAGS	+= --std=c++1z
override CPPFLAGS	+= -MMD -MP
override CPPFLAGS	+= -I../source -I../include
override CPPFLAGS	+= $(shell cat ../.cxxflags 2> /dev/null | xargs)
override LDFLAGS	+= -L../out/ -L../lib/
override LDFLAGS	+= $(shell cat .ldflags 2> /dev/null | xargs)

override LDARCH		+= $(shell cat .ldarch 2> /dev/null | xargs)
ifeq ($(strip $(LDARCH)),)
override LDLIBS		+= -lsimple_sdlcore
else
override LDLIBS		+= $(LDARCH)
endif

override LDLIBS		+= -lsimple_interactive -lSDL2main -lSDL2

TEMPDIR	:= temp
DISTDIR	:= out

SOURCES	:= $(shell echo *.cpp)
TARGETS	:= $(SOURCES:%.cpp=$(DISTDIR)/%)
OBJECTS	:= $(SOURCES:%.cpp=$(TEMPDIR)/%.o)
DEPENDS	:= $(OBJECTS:.o=.d)

build: make_parent $(TARGETS)

make_parent:
	make -C ..

$(DISTDIR)/%: $(TEMPDIR)/%.o ../out/libsimple_interactive.a $(LDARCH) | $(DISTDIR)
	$(CXX) $(LDFLAGS) $< $(LDLIBS) -o $@

$(TEMPDIR)/%.o: %.cpp | $(TEMPDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(TEMPDIR):
	@mkdir $@

$(DISTDIR):
	@mkdir $@

clean:
	@rm $(DEPENDS) 2> /dev/null || true
	@rm $(OBJECTS) 2> /dev/null || true
	@rmdir $(TEMPDIR) 2> /dev/null || true
	@echo Temporaries cleaned!

distclean: clean
	@rm $(TARGETS) 2> /dev/null || true
	@rmdir $(DISTDIR) 2> /dev/null || true
	@echo All clean!

-include $(DEPENDS)

.PRECIOUS : $(OBJECTS)
.PHONY : clean distclean make_parent
