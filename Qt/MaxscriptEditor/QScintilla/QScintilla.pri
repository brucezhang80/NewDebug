!win32:VERSION = 9.0.2

#TEMPLATE = lib
#TARGET = qscintilla2
CONFIG += qt warn_off release dll thread
INCLUDEPATH = $$PWD/include $$PWD/lexlib $$PWD/src $$PWD/Qt $$PWD/Qt/Qsci
DEFINES = QSCINTILLA_MAKE_DLL QT SCI_LEXER

#greaterThan(QT_MAJOR_VERSION, 4)
#{
#	QT += widgets
#	QT += printsupport
#}

# Handle both Qt v4 and v3.
#target.path = $$[QT_INSTALL_LIBS]
#isEmpty(target.path) {
#        target.path = $(QTDIR)/lib
#}

#header.path = $$[QT_INSTALL_HEADERS]
#header.files = Qsci
#isEmpty(header.path) {
#        header.path = $(QTDIR)/include/Qsci
#        header.files = Qsci/qsci*.h
#}

#trans.path = $$[QT_INSTALL_TRANSLATIONS]
#trans.files = qscintilla_*.qm
#isEmpty(trans.path) {
#        trans.path = $(QTDIR)/translations
#}

#qsci.path = $$[QT_INSTALL_DATA]
#qsci.files = ./qsci
#isEmpty(qsci.path) {
#        qsci.path = $(QTDIR)
#}

#INSTALLS += header trans qsci target

HEADERS = \
    QScintilla/src/XPM.h \
    QScintilla/src/ViewStyle.h \
    QScintilla/src/UniConversion.h \
    QScintilla/src/SVector.h \
    QScintilla/src/Style.h \
    QScintilla/src/SplitVector.h \
    QScintilla/src/Selection.h \
    QScintilla/src/ScintillaBase.h \
    QScintilla/src/RunStyles.h \
    QScintilla/src/RESearch.h \
    QScintilla/src/PositionCache.h \
    QScintilla/src/PerLine.h \
    QScintilla/src/Partitioning.h \
    QScintilla/src/LineMarker.h \
    QScintilla/src/KeyMap.h \
    QScintilla/src/Indicator.h \
    QScintilla/src/FontQuality.h \
    QScintilla/src/ExternalLexer.h \
    QScintilla/src/Editor.h \
    QScintilla/src/Document.h \
    QScintilla/src/Decoration.h \
    QScintilla/src/ContractionState.h \
    QScintilla/src/CharClassify.h \
    QScintilla/src/CellBuffer.h \
    QScintilla/src/Catalogue.h \
    QScintilla/src/CallTip.h \
    QScintilla/src/AutoComplete.h \
    QScintilla/include/ScintillaWidget.h \
    QScintilla/include/Scintilla.h \
    QScintilla/include/SciLexer.h \
    QScintilla/include/Platform.h \
    QScintilla/include/ILexer.h \
    QScintilla/lexlib/WordList.h \
    QScintilla/lexlib/StyleContext.h \
    QScintilla/lexlib/SparseState.h \
    QScintilla/lexlib/PropSetSimple.h \
    QScintilla/lexlib/OptionSet.h \
    QScintilla/lexlib/LexerSimple.h \
    QScintilla/lexlib/LexerNoExceptions.h \
    QScintilla/lexlib/LexerModule.h \
    QScintilla/lexlib/LexerBase.h \
    QScintilla/lexlib/LexAccessor.h \
    QScintilla/lexlib/CharacterSet.h \
    QScintilla/lexlib/Accessor.h \
    QScintilla/Qt/ScintillaQt.h \
    QScintilla/Qt/SciNamespace.h \
    QScintilla/Qt/SciClasses.h \
    QScintilla/Qt/ListBoxQt.h \
    QScintilla/Qt/Qsci/qscistyledtext.h \
    QScintilla/Qt/Qsci/qscistyle.h \
    QScintilla/Qt/Qsci/qsciscintillabase.h \
    QScintilla/Qt/Qsci/qsciscintilla.h \
    QScintilla/Qt/Qsci/qsciprinter.h \
    QScintilla/Qt/Qsci/qscimacro.h \
    QScintilla/Qt/Qsci/qscilexeryaml.h \
    QScintilla/Qt/Qsci/qscilexerxml.h \
    QScintilla/Qt/Qsci/qscilexervhdl.h \
    QScintilla/Qt/Qsci/qscilexerverilog.h \
    QScintilla/Qt/Qsci/qscilexertex.h \
    QScintilla/Qt/Qsci/qscilexertcl.h \
    QScintilla/Qt/Qsci/qscilexersql.h \
    QScintilla/Qt/Qsci/qscilexerspice.h \
    QScintilla/Qt/Qsci/qscilexerruby.h \
    QScintilla/Qt/Qsci/qscilexerpython.h \
    QScintilla/Qt/Qsci/qscilexerproperties.h \
    QScintilla/Qt/Qsci/qscilexerpov.h \
    QScintilla/Qt/Qsci/qscilexerpostscript.h \
    QScintilla/Qt/Qsci/qscilexerperl.h \
    QScintilla/Qt/Qsci/qscilexerpascal.h \
    QScintilla/Qt/Qsci/qscilexeroctave.h \
    QScintilla/Qt/Qsci/qscilexermaxscript.h \
    QScintilla/Qt/Qsci/qscilexermatlab.h \
    QScintilla/Qt/Qsci/qscilexermakefile.h \
    QScintilla/Qt/Qsci/qscilexerlua.h \
    QScintilla/Qt/Qsci/qscilexerjavascript.h \
    QScintilla/Qt/Qsci/qscilexerjava.h \
    QScintilla/Qt/Qsci/qscilexeridl.h \
    QScintilla/Qt/Qsci/qscilexerhtml.h \
    QScintilla/Qt/Qsci/qscilexerfortran77.h \
    QScintilla/Qt/Qsci/qscilexerfortran.h \
    QScintilla/Qt/Qsci/qscilexerdiff.h \
    QScintilla/Qt/Qsci/qscilexerd.h \
    QScintilla/Qt/Qsci/qscilexercustom.h \
    QScintilla/Qt/Qsci/qscilexercss.h \
    QScintilla/Qt/Qsci/qscilexercsharp.h \
    QScintilla/Qt/Qsci/qscilexercpp.h \
    QScintilla/Qt/Qsci/qscilexercmake.h \
    QScintilla/Qt/Qsci/qscilexerbatch.h \
    QScintilla/Qt/Qsci/qscilexerbash.h \
    QScintilla/Qt/Qsci/qscilexer.h \
    QScintilla/Qt/Qsci/qsciglobal.h \
    QScintilla/Qt/Qsci/qscidocument.h \
    QScintilla/Qt/Qsci/qscicommandset.h \
    QScintilla/Qt/Qsci/qscicommand.h \
    QScintilla/Qt/Qsci/qsciapis.h \
    QScintilla/Qt/Qsci/qsciabstractapis.h

SOURCES = \
    QScintilla/src/XPM.cpp \
    QScintilla/src/ViewStyle.cpp \
    QScintilla/src/UniConversion.cpp \
    QScintilla/src/Style.cpp \
    QScintilla/src/Selection.cpp \
    QScintilla/src/ScintillaBase.cpp \
    QScintilla/src/RunStyles.cpp \
    QScintilla/src/RESearch.cpp \
    QScintilla/src/PositionCache.cpp \
    QScintilla/src/PerLine.cpp \
    QScintilla/src/LineMarker.cpp \
    QScintilla/src/KeyMap.cpp \
    QScintilla/src/Indicator.cpp \
    QScintilla/src/ExternalLexer.cpp \
    QScintilla/src/Editor.cpp \
    QScintilla/src/Document.cpp \
    QScintilla/src/Decoration.cpp \
    QScintilla/src/ContractionState.cpp \
    QScintilla/src/CharClassify.cpp \
    QScintilla/src/CellBuffer.cpp \
    QScintilla/src/Catalogue.cpp \
    QScintilla/src/CallTip.cpp \
    QScintilla/src/AutoComplete.cpp \
    QScintilla/lexers/LexYAML.cpp \
    QScintilla/lexers/LexVisualProlog.cpp \
    QScintilla/lexers/LexVHDL.cpp \
    QScintilla/lexers/LexVerilog.cpp \
    QScintilla/lexers/LexVB.cpp \
    QScintilla/lexers/LexTxt2tags.cpp \
    QScintilla/lexers/LexTeX.cpp \
    QScintilla/lexers/LexTCMD.cpp \
    QScintilla/lexers/LexTCL.cpp \
    QScintilla/lexers/LexTAL.cpp \
    QScintilla/lexers/LexTADS3.cpp \
    QScintilla/lexers/LexTACL.cpp \
    QScintilla/lexers/LexSQL.cpp \
    QScintilla/lexers/LexSpice.cpp \
    QScintilla/lexers/LexSpecman.cpp \
    QScintilla/lexers/LexSorcus.cpp \
    QScintilla/lexers/LexSML.cpp \
    QScintilla/lexers/LexSmalltalk.cpp \
    QScintilla/lexers/LexScriptol.cpp \
    QScintilla/lexers/LexRuby.cpp \
    QScintilla/lexers/LexRebol.cpp \
    QScintilla/lexers/LexR.cpp \
    QScintilla/lexers/LexPython.cpp \
    QScintilla/lexers/LexPS.cpp \
    QScintilla/lexers/LexProgress.cpp \
    QScintilla/lexers/LexPowerShell.cpp \
    QScintilla/lexers/LexPowerPro.cpp \
    QScintilla/lexers/LexPOV.cpp \
    QScintilla/lexers/LexPO.cpp \
    QScintilla/lexers/LexPLM.cpp \
    QScintilla/lexers/LexPerl.cpp \
    QScintilla/lexers/LexPB.cpp \
    QScintilla/lexers/LexPascal.cpp \
    QScintilla/lexers/LexOthers.cpp \
    QScintilla/lexers/LexOScript.cpp \
    QScintilla/lexers/LexOpal.cpp \
    QScintilla/lexers/LexNsis.cpp \
    QScintilla/lexers/LexNimrod.cpp \
    QScintilla/lexers/LexMySQL.cpp \
    QScintilla/lexers/LexMSSQL.cpp \
    QScintilla/lexers/LexMPT.cpp \
    QScintilla/lexers/LexModula.cpp \
    QScintilla/lexers/LexMMIXAL.cpp \
    QScintilla/lexers/LexMetapost.cpp \
    QScintilla/lexers/LexMAXscript.cpp \
    QScintilla/lexers/LexMatlab.cpp \
    QScintilla/lexers/LexMarkdown.cpp \
    QScintilla/lexers/LexMagik.cpp \
    QScintilla/lexers/LexLua.cpp \
    QScintilla/lexers/LexLout.cpp \
    QScintilla/lexers/LexLisp.cpp \
    QScintilla/lexers/LexKix.cpp \
    QScintilla/lexers/LexInno.cpp \
    QScintilla/lexers/LexHTML.cpp \
    QScintilla/lexers/LexHaskell.cpp \
    QScintilla/lexers/LexGui4Cli.cpp \
    QScintilla/lexers/LexGAP.cpp \
    QScintilla/lexers/LexFortran.cpp \
    QScintilla/lexers/LexForth.cpp \
    QScintilla/lexers/LexFlagship.cpp \
    QScintilla/lexers/LexEScript.cpp \
    QScintilla/lexers/LexErlang.cpp \
    QScintilla/lexers/LexEiffel.cpp \
    QScintilla/lexers/LexECL.cpp \
    QScintilla/lexers/LexD.cpp \
    QScintilla/lexers/LexCSS.cpp \
    QScintilla/lexers/LexCsound.cpp \
    QScintilla/lexers/LexCrontab.cpp \
    QScintilla/lexers/LexCPP.cpp \
    QScintilla/lexers/LexConf.cpp \
    QScintilla/lexers/LexCoffeeScript.cpp \
    QScintilla/lexers/LexCOBOL.cpp \
    QScintilla/lexers/LexCmake.cpp \
    QScintilla/lexers/LexCLW.cpp \
    QScintilla/lexers/LexCaml.cpp \
    QScintilla/lexers/LexBullant.cpp \
    QScintilla/lexers/LexBasic.cpp \
    QScintilla/lexers/LexBash.cpp \
    QScintilla/lexers/LexBaan.cpp \
    QScintilla/lexers/LexAVS.cpp \
    QScintilla/lexers/LexAVE.cpp \
    QScintilla/lexers/LexAU3.cpp \
    QScintilla/lexers/LexASY.cpp \
    QScintilla/lexers/LexAsn1.cpp \
    QScintilla/lexers/LexAsm.cpp \
    QScintilla/lexers/LexAPDL.cpp \
    QScintilla/lexers/LexAda.cpp \
    QScintilla/lexers/LexAbaqus.cpp \
    QScintilla/lexers/LexA68k.cpp \
    QScintilla/lexlib/WordList.cpp \
    QScintilla/lexlib/StyleContext.cpp \
    QScintilla/lexlib/PropSetSimple.cpp \
    QScintilla/lexlib/LexerSimple.cpp \
    QScintilla/lexlib/LexerNoExceptions.cpp \
    QScintilla/lexlib/LexerModule.cpp \
    QScintilla/lexlib/LexerBase.cpp \
    QScintilla/lexlib/CharacterSet.cpp \
    QScintilla/lexlib/Accessor.cpp \
    QScintilla/Qt/ScintillaQt.cpp \
    QScintilla/Qt/SciClasses.cpp \
    QScintilla/Qt/qscistyledtext.cpp \
    QScintilla/Qt/qscistyle.cpp \
    QScintilla/Qt/qsciscintillabase.cpp \
    QScintilla/Qt/qsciscintilla.cpp \
    QScintilla/Qt/qsciprinter.cpp \
    QScintilla/Qt/qscimacro.cpp \
    QScintilla/Qt/qscilexeryaml.cpp \
    QScintilla/Qt/qscilexerxml.cpp \
    QScintilla/Qt/qscilexervhdl.cpp \
    QScintilla/Qt/qscilexerverilog.cpp \
    QScintilla/Qt/qscilexertex.cpp \
    QScintilla/Qt/qscilexertcl.cpp \
    QScintilla/Qt/qscilexersql.cpp \
    QScintilla/Qt/qscilexerspice.cpp \
    QScintilla/Qt/qscilexerruby.cpp \
    QScintilla/Qt/qscilexerpython.cpp \
    QScintilla/Qt/qscilexerproperties.cpp \
    QScintilla/Qt/qscilexerpov.cpp \
    QScintilla/Qt/qscilexerpostscript.cpp \
    QScintilla/Qt/qscilexerperl.cpp \
    QScintilla/Qt/qscilexerpascal.cpp \
    QScintilla/Qt/qscilexeroctave.cpp \
    QScintilla/Qt/qscilexermaxscript.cpp \
    QScintilla/Qt/qscilexermatlab.cpp \
    QScintilla/Qt/qscilexermakefile.cpp \
    QScintilla/Qt/qscilexerlua.cpp \
    QScintilla/Qt/qscilexerjavascript.cpp \
    QScintilla/Qt/qscilexerjava.cpp \
    QScintilla/Qt/qscilexeridl.cpp \
    QScintilla/Qt/qscilexerhtml.cpp \
    QScintilla/Qt/qscilexerfortran77.cpp \
    QScintilla/Qt/qscilexerfortran.cpp \
    QScintilla/Qt/qscilexerdiff.cpp \
    QScintilla/Qt/qscilexerd.cpp \
    QScintilla/Qt/qscilexercustom.cpp \
    QScintilla/Qt/qscilexercss.cpp \
    QScintilla/Qt/qscilexercsharp.cpp \
    QScintilla/Qt/qscilexercpp.cpp \
    QScintilla/Qt/qscilexercmake.cpp \
    QScintilla/Qt/qscilexerbatch.cpp \
    QScintilla/Qt/qscilexerbash.cpp \
    QScintilla/Qt/qscilexer.cpp \
    QScintilla/Qt/qscidocument.cpp \
    QScintilla/Qt/qscicommandset.cpp \
    QScintilla/Qt/qscicommand.cpp \
    QScintilla/Qt/qsciapis.cpp \
    QScintilla/Qt/qsciabstractapis.cpp \
    QScintilla/Qt/PlatQt.cpp \
    QScintilla/Qt/ListBoxQt.cpp

TRANSLATIONS = \
	Qt/qscintilla_cs.ts \
	Qt/qscintilla_de.ts \
	Qt/qscintilla_es.ts \
	Qt/qscintilla_fr.ts \
	Qt/qscintilla_pt_br.ts \
	Qt/qscintilla_ru.ts

OTHER_FILES += \
    QScintilla/src/SciTE.properties \
    QScintilla/src/LexGen.py \
    QScintilla/include/License.txt \
    QScintilla/include/HFacer.py \
    QScintilla/include/Face.py \
    QScintilla/include/Scintilla.iface \
    QScintilla/Qt/qscintilla_ru.qm \
    QScintilla/Qt/qscintilla_pt_br.qm \
    QScintilla/Qt/qscintilla_fr.qm \
    QScintilla/Qt/qscintilla_es.qm \
    QScintilla/Qt/qscintilla_de.qm \
    QScintilla/Qt/qscintilla_cs.qm \
    QScintilla/Qt/qscintilla_ru.ts \
    QScintilla/Qt/qscintilla_pt_br.ts \
    QScintilla/Qt/qscintilla_fr.ts \
    QScintilla/Qt/qscintilla_es.ts \
    QScintilla/Qt/qscintilla_de.ts \
    QScintilla/Qt/qscintilla_cs.ts
