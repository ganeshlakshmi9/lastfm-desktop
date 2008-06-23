#!/bin/bash

windows()
{
    # we step into src because qmake is b0rked
    cd src
    qmake -recursive -tp vc pigmyshrew.pro

    AFTER='
Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "Solution Items", "Solution Items", "{780C13D3-8172-4EE0-8FD5-26ED6489851C}"
	ProjectSection(SolutionItems) = preProject
		include.pro = include.pro
		.qmake.cache = .qmake.cache
		pigmyshrew.pro = pigmyshrew.pro
	EndProjectSection
EndProject
Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "common", "common", "{3FDD67B7-DF67-4F22-8545-755D722794BC}"
	ProjectSection(SolutionItems) = preProject
		common\DllExportMacro.h = common\DllExportMacro.h
	EndProjectSection
EndProject
'

    echo "$AFTER" >> pigmyshrew.sln

    rmdir _build
}

osx() 
{
    echo unimplemented
}

linux()
{
    echo unimplemented
}


case `uname` in
    Darwin) osx;;
    Linux) linux;;
    *) windows;;
esac
