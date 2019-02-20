##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=mumtp
ConfigurationName      :=Debug
WorkspacePath          := "./"
ProjectPath            := "./"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=root
Date                   :=02/20/19
CodeLitePath           :="/root/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="mumtp.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)usb-1.0 
ArLibs                 :=  "usb-1.0" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/mtpPacket.cpp$(ObjectSuffix) $(IntermediateDirectory)/mtpOperator.cpp$(ObjectSuffix) $(IntermediateDirectory)/byteVector.cpp$(ObjectSuffix) $(IntermediateDirectory)/logger.cpp$(ObjectSuffix) $(IntermediateDirectory)/pathHandler.cpp$(ObjectSuffix) $(IntermediateDirectory)/fileWriter.cpp$(ObjectSuffix) $(IntermediateDirectory)/phoneDevice.cpp$(ObjectSuffix) $(IntermediateDirectory)/usbDevice.cpp$(ObjectSuffix) $(IntermediateDirectory)/storageInfo.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/objectInfo.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/mtpPacket.cpp$(ObjectSuffix): mtpPacket.cpp $(IntermediateDirectory)/mtpPacket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//mtpPacket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mtpPacket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mtpPacket.cpp$(DependSuffix): mtpPacket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mtpPacket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mtpPacket.cpp$(DependSuffix) -MM "mtpPacket.cpp"

$(IntermediateDirectory)/mtpPacket.cpp$(PreprocessSuffix): mtpPacket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mtpPacket.cpp$(PreprocessSuffix) "mtpPacket.cpp"

$(IntermediateDirectory)/mtpOperator.cpp$(ObjectSuffix): mtpOperator.cpp $(IntermediateDirectory)/mtpOperator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//mtpOperator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mtpOperator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mtpOperator.cpp$(DependSuffix): mtpOperator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mtpOperator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mtpOperator.cpp$(DependSuffix) -MM "mtpOperator.cpp"

$(IntermediateDirectory)/mtpOperator.cpp$(PreprocessSuffix): mtpOperator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mtpOperator.cpp$(PreprocessSuffix) "mtpOperator.cpp"

$(IntermediateDirectory)/byteVector.cpp$(ObjectSuffix): byteVector.cpp $(IntermediateDirectory)/byteVector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//byteVector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/byteVector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/byteVector.cpp$(DependSuffix): byteVector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/byteVector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/byteVector.cpp$(DependSuffix) -MM "byteVector.cpp"

$(IntermediateDirectory)/byteVector.cpp$(PreprocessSuffix): byteVector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/byteVector.cpp$(PreprocessSuffix) "byteVector.cpp"

$(IntermediateDirectory)/logger.cpp$(ObjectSuffix): logger.cpp $(IntermediateDirectory)/logger.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//logger.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/logger.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/logger.cpp$(DependSuffix): logger.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/logger.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/logger.cpp$(DependSuffix) -MM "logger.cpp"

$(IntermediateDirectory)/logger.cpp$(PreprocessSuffix): logger.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/logger.cpp$(PreprocessSuffix) "logger.cpp"

$(IntermediateDirectory)/pathHandler.cpp$(ObjectSuffix): pathHandler.cpp $(IntermediateDirectory)/pathHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//pathHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pathHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pathHandler.cpp$(DependSuffix): pathHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pathHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/pathHandler.cpp$(DependSuffix) -MM "pathHandler.cpp"

$(IntermediateDirectory)/pathHandler.cpp$(PreprocessSuffix): pathHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pathHandler.cpp$(PreprocessSuffix) "pathHandler.cpp"

$(IntermediateDirectory)/fileWriter.cpp$(ObjectSuffix): fileWriter.cpp $(IntermediateDirectory)/fileWriter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//fileWriter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fileWriter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fileWriter.cpp$(DependSuffix): fileWriter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fileWriter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/fileWriter.cpp$(DependSuffix) -MM "fileWriter.cpp"

$(IntermediateDirectory)/fileWriter.cpp$(PreprocessSuffix): fileWriter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fileWriter.cpp$(PreprocessSuffix) "fileWriter.cpp"

$(IntermediateDirectory)/phoneDevice.cpp$(ObjectSuffix): phoneDevice.cpp $(IntermediateDirectory)/phoneDevice.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//phoneDevice.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/phoneDevice.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/phoneDevice.cpp$(DependSuffix): phoneDevice.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/phoneDevice.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/phoneDevice.cpp$(DependSuffix) -MM "phoneDevice.cpp"

$(IntermediateDirectory)/phoneDevice.cpp$(PreprocessSuffix): phoneDevice.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/phoneDevice.cpp$(PreprocessSuffix) "phoneDevice.cpp"

$(IntermediateDirectory)/usbDevice.cpp$(ObjectSuffix): usbDevice.cpp $(IntermediateDirectory)/usbDevice.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//usbDevice.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/usbDevice.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/usbDevice.cpp$(DependSuffix): usbDevice.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/usbDevice.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/usbDevice.cpp$(DependSuffix) -MM "usbDevice.cpp"

$(IntermediateDirectory)/usbDevice.cpp$(PreprocessSuffix): usbDevice.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/usbDevice.cpp$(PreprocessSuffix) "usbDevice.cpp"

$(IntermediateDirectory)/storageInfo.cpp$(ObjectSuffix): storageInfo.cpp $(IntermediateDirectory)/storageInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//storageInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storageInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storageInfo.cpp$(DependSuffix): storageInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storageInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storageInfo.cpp$(DependSuffix) -MM "storageInfo.cpp"

$(IntermediateDirectory)/storageInfo.cpp$(PreprocessSuffix): storageInfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storageInfo.cpp$(PreprocessSuffix) "storageInfo.cpp"

$(IntermediateDirectory)/objectInfo.cpp$(ObjectSuffix): objectInfo.cpp $(IntermediateDirectory)/objectInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) ".//objectInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/objectInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/objectInfo.cpp$(DependSuffix): objectInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/objectInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/objectInfo.cpp$(DependSuffix) -MM "objectInfo.cpp"

$(IntermediateDirectory)/objectInfo.cpp$(PreprocessSuffix): objectInfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/objectInfo.cpp$(PreprocessSuffix) "objectInfo.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


