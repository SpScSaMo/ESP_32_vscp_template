#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := vscp_project

include $(IDF_PATH)/make/project.mk

# GESONDERT HINZUGEFUEGT
EXTRA_COMPONENT_DIRS= $(PROJECT_PATH)/components/vscp_firmware/common