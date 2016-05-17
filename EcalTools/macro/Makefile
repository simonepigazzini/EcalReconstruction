ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs) -lTMVA

CXX           = g++
CXXFLAGS      = -g -fPIC -Wno-deprecated -O -ansi -D_GNU_SOURCE -g -O2
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared


ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)


CXXFLAGS      += $(ROOTCFLAGS)
#CXX           += -I./
LIBS           = $(ROOTLIBS) 

NGLIBS         = $(ROOTGLIBS) 
NGLIBS        += -lMinuit 
GLIBS          = $(filter-out -lNew, $(NGLIBS))


INCLUDEDIR           = ./
OUTLIB               = ./lib/

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ./lib/

# the main executable
ecalhitana: $(INCLUDEDIR)/ecalhitana.cc \
	$(OUTLIB)ecaltree.o \
	$(OUTLIB)pusubtree.o \
	$(OUTLIB)MultifitValidation.o
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -ldl -o ecalhitana $(OUTLIB)/*.o $(GLIBS) $(LDFLAGS) $ $<

# Analysis generic libs
$(OUTLIB)ecaltree.o: $(INCLUDEDIR)/ecaltree.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ecaltree.o $<

$(OUTLIB)pusubtree.o: $(INCLUDEDIR)/pusubtree.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)pusubtree.o $<

$(OUTLIB)MultifitValidation.o: $(INCLUDEDIR)/MultifitValidation.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)MultifitValidation.o $<

clean:
	rm -f lib/*o
	rm -f ecalhitana
