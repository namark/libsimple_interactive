override	CPPFLAGS	+= --std=c++1z
override	CPPFLAGS	+= -MMD -MP
override	CPPFLAGS	+= -I./include
override	CPPFLAGS	+= $(shell cat .cxxflags 2> /dev/null | xargs)

ARFLAGS	:= $(ARFLAGS)c

PREFIX	:= $(DESTDIR)/usr/local
INCDIR	:= $(PREFIX)/include
LIBDIR	:= $(PREFIX)/lib

SRCDIR	:= ./source
TEMPDIR	:= temp
DISTDIR	:= out
TARGET	:= libsimple_interactive.a
OUT		:= $(DISTDIR)/$(TARGET)
SOURCES	:= $(shell find -wholename "$(SRCDIR)/*.cpp")
HEADERS	:= $(shell find -wholename "$(SRCDIR)/*.hpp" && find -wholename "$(SRCDIR)/*.h")
INCLUDE	:= $(HEADERS:$(SRCDIR)/%=$(INCDIR)/%)
INCDIRS	:= $(shell dirname $(INCLUDE))
OBJECTS	:= $(SOURCES:$(SRCDIR)/%.cpp=$(TEMPDIR)/%.o)
OBJDIRS	:= $(shell dirname $(OBJECTS))
DEPENDS	:= $(OBJECTS:.o=.d)


$(OUT): $(OBJECTS) | $(DISTDIR)
	$(AR) $(ARFLAGS) $@ $^

$(TEMPDIR)/%.o: $(SRCDIR)/%.cpp | $(TEMPDIR)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(TEMPDIR):
	@mkdir $@

$(DISTDIR):
	@mkdir $@

clean:
	@rm $(DEPENDS) 2> /dev/null || true
	@rm $(OBJECTS) 2> /dev/null || true
	@rmdir -p $(OBJDIRS) 2> /dev/null || true
	@echo Temporaries cleaned!

distclean: clean
	@rm $(OUT) 2> /dev/null || true
	@rmdir $(DISTDIR) 2> /dev/null || true
	@echo All clean!

install: $(LIBDIR)/$(TARGET) $(INCLUDE)

$(LIBDIR)/$(TARGET): $(OUT) | $(LIBDIR)
	cp $< $@

$(LIBDIR):
	@mkdir $@

$(INCDIR)/%.h: $(SRCDIR)/%.h
	@mkdir -p $(@D)
	cp $< $@

$(INCDIR)/%.hpp: $(SRCDIR)/%.hpp
	@mkdir -p $(@D)
	cp $< $@

uninstall:
	-rm $(INCLUDE)
	@rmdir -p $(INCDIRS) 2> /dev/null || true
	-rm $(LIBDIR)/$(TARGET)
	@rmdir $(LIBDIR) 2> /dev/null || true
	@echo Uninstall complete!

-include $(DEPENDS)

.PRECIOUS : $(OBJECTS)
.PHONY : clean distclean uninstall
