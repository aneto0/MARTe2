${header_text}

SPB = ${spb}

SPBT = ${spbt}

OBJSX = ${objsx}

${package}

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

ROOT_DIR = ${root_dir}

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

${libraries}

${includes}

${libraries_static}

${target_1}

${target_2}

${target_3}

${target_4}

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

check-env:
ifndef MARTe2_DIR
    $(error MARTe2_DIR is undefined)
endif
ifndef MARTe2_Components_DIR
    $(error MARTe2_Components_DIR is undefined)
endif

