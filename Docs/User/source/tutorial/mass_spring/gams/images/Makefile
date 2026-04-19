#gv files automatically created with ~/Projects/MARTe2CfgToGraphviz/Build/linux/Source/CfgToDot.ex ~/Projects/Fast-Control-Falcon/Configurations/BareConfiguration-2019_06_13.cfg -o FalconFastControl

OUTPUT=png
IMAGES:= $(patsubst %.gv,%.$(OUTPUT),$(wildcard *.gv))

PLANTUML_SRC := $(wildcard *.puml)
PLANTUML_PNG := $(addsuffix .png, $(basename $(PLANTUML_SRC)))

%.$(OUTPUT): %.gv
	dot -T$(OUTPUT) $< -o $@

%.png: %.puml
	java -jar $(PLANTUML_JAR) -tpng $^

all: check_plantuml_jar $(IMAGES) $(PLANTUML_PNG)

clean:
	rm $(IMAGES) $(PLANTUML_PNG)

check_plantuml_jar:
ifndef PLANTUML_JAR
#Note the tab
	$(error PLANTUML_JAR is undefined)
endif 
