#include "Qsci/qscilexermaxscript.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

QsciLexerMAXScript::QsciLexerMAXScript(QObject* parent):
    QsciLexer(parent),
    fold_comments(false),
    fold_compact(false),
    fold_atelse(false),
    fold_atbegin(false),
    fold_atparenth(false)
{
}

QsciLexerMAXScript::~QsciLexerMAXScript()
{

}

const char *QsciLexerMAXScript::language() const
{
    return "MAXScript";
}

const char *QsciLexerMAXScript::lexer() const
{
    return "maxscript";
}

int QsciLexerMAXScript::braceStyle() const
{
    return Attribute;
}

QStringList QsciLexerMAXScript::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << "::" << ".";

    return wl;
}

const char *QsciLexerMAXScript::blockStartKeyword(int *style) const
{
    if (style)
        *style = Keyword;

    return "case catch class default do then else of on with for if loop"
           "private protected public struct try while utility rollout tool rcmenu"
            "macroscript plugin";
}

const char *QsciLexerMAXScript::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "(";
}

const char *QsciLexerMAXScript::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return ")";
}

QColor QsciLexerMAXScript::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x00,0x80);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case CommentLine:
        return QColor(0x3f,0x7f,0x3f);

    case Number:
    case StandardOperator:
        return QColor(0x00,0x7f,0x7f);

    case String:
        return QColor(0x7f,0x00,0x7f);

    case UnclosedString:
        return QColor(0x00,0x00,0x00);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case Attribute:
        return QColor(0x80,0x40,0x20);

    case StandardFunction:
        return QColor(0x80,0x80,0x20);

    case StandardPackage:
        return QColor(0x20,0x80,0x20);

    case StandardType:
        return QColor(0x20,0x80,0x80);

    case KeywordSet7:
        return QColor(0x80,0x40,0x20);
    }

    return QsciLexer::defaultColor(style);
}

bool QsciLexerMAXScript::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}

QFont QsciLexerMAXScript::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
    case KeywordSet7:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}

QColor QsciLexerMAXScript::defaultPaper(int style) const
{
    if (style == UnclosedString)
        return QColor(0xe0,0xc0,0xe0);

    return QsciLexer::defaultPaper(style);
}

const char *QsciLexerMAXScript::keywords(int set) const
{
    if (set == 1)
        return
            "about and animate as at"
            "by"
            "case catch collect continue coordsys"
            "do"
            "else exit"
            "fn for from function"
            "global"
            "if in"
            "local"
            "macroscript mapped max material"
            "name not"
            "of off on or"
            "parameters persistent plugin prefix pos pivot parent"
            "rcmenu return rollout rotation"
            "set struct scale"
            "target then throw to tool try transform"
            "undo utility"
            "when where while with";

    if (set == 2)
        return
            "MAX"
            "activeGrid altKey ambientColor ambientColorController animButtonEnabled animButtonState animationRange"
            "backgroundColor backgroundColorController backgroundImageFileName black blue brown"
            "ctrlKey cui.commandPanelOpen currentMaterialLibrary currentTime"
            "displayGamma displaySafeFrames doDisabled dontcollect"
            "editorFont editorFontSize editorTabWidth environmentMap escapeEnable"
            "false fileInGamma fileOutGamma flyOffTime frameRate"
            "globalTracks gray green gridDist"
            "hardwareLockID heapFree heapSize hotspotAngleSeparation"
            "inputTextColor"
            "keyboard.Pressed keyboard.altPressed keyboard.controlPressed"
            "lightLevel lightLevelController lightTintColor lightTintColorController localTime logsystem.quietmode"
            "mButton maxFileName maxFilePath meditMaterials mesh messageTextColor"
            "nodeTM numAtmospherics numEffects numSubObjectLevels"
            "off ok on orange outputTextColor"
            "playActiveOnly preferences.constantReferenceSystem preferences.flyOffTime preferences.maximumGBufferLayer"
            "preferences.spinnerWrap preferences.useLargeVertexDots preferences.useTransformGizmos preferences.useVertexDots"
            "rButton realTimePlayback red rendOutputFilename renderDisplacements renderEffects renderHeight"
            "renderPixelAspect renderWidth renderer rootNode"
            "scanlineRender.antiAliasFilter scanlineRender.antiAliasFilterSize scanlineRender.enablePixelSampler"
            "sceneMaterials scriptsPath seEnvironmentMap selectionSets shiftKey showEndResult skipRenderedFrames"
            "sliderTime snapMode.active snapMode.type stackLimit subObjectLevel"
            "ticksPerFrame timeConfiguration.playActiveOnly timeConfiguration.realTimePlayback timeConfiguration.useTrackBar"
            "trackViewNodes trackbar.filter trackbar.visible true"
            "undefined unsupplied"
            "videoPostTracks viewPoint viewport.activeViewport viewport.numViews"
            "white worldAngle worldPoint"
            "yellow";

    if (set == 3)
        return
            "-- $ \\ & { }"
            "buildMesh"
            "changed close create"
            "entered"
            "moved"
            "open"
            "picked preApply pressed"
            "resized"
            "selected simpleObject"
            "controller"
            ".value .range .text .checked .count .enabled .diffuseMap .time .selected .inTangent .outTangent .inTangentType .outTangentType .x_locked .y_locked .z_locked .constantVelocity .steps"
            ".optimize .thickness .mapCoords .adaptive .renderable .radius .segs .smooth .mapCoords  .material .name .OpacityMap .rgbOutput";

    if (set == 4)
        return
            "alias: animatable:"
            "checked: classID: controller:"
            "default:"
            "enabled: errPrompt: extends:"
            "fieldwidth: filter: framerange:"
            "handleAt: height: highlightColor:"
            "id: invisible:"
            "keyword:"
            "label:"
            "map: material:"
            "name:"
            "orient: outputwidth:"
            "pos: prompt:"
            "range: replaceUI: rolledUp:"
            "scale: silentErrors: snap: subAnim:"
            "tabSize: tabSizeVariable: terminators: ticks: tooltip: type:"
            "ui:"
            "version: vfb:"
            "width:";

    if (set == 5)
        return
            "#2D #2_5D #3D #TMOnly #all #allEdges #allkeys #angle #angleTab #animatedTracks #arrow #asterisk #backcull #baseObjects #bigBox #bitmapTab #blue #boolTab #boolean #boxMode #Bitmaptexture"
            "#center #circle #clear #color #colorChannel #colorChannelTab #colorTab #colorVerts #constant #contents #controllerTypes #coverage #create #currentTM #curveX #curveY #curveZ"
            "#custom #cycle #default #diamond #display #draft #driverConfig #dualPlanes #fast #filename #filenameTab #fill #flat #float #floatTab #geomAccel #green #grid #hideCameras"
            "#hideGeometry #hideHelpers #hideLights #hideShapes #hideSpacewarps #hieracthy #hierarchy #hollowBox #horizontal #hybrid #illum #image #inactive #incNone #incrUpdate"
            "#instanceOnly #intTab #integer #left #lighting #line #linear #local #localOnly #loop #maps #mat #matID #material #materialParameters #materialTab #materialsMaps #mats"
            "#maxObject #maxObjectTab #mergeDups #missing #modifiedObjects #modify #motion #mtlEditor #noAtts #noRedraw #node #nodeTab #none #normal #noteTracks #nuscale #object #objectID"
            "#objects #other #paintDoesBlit #parent #passDecal #percentTab #perspCorrect #pick #pingpong #plusSign #point #point3 #point3Tab #polyEdges #position #positionX #positionY"
            "#positionZ #production #prompt #promptDups #radiobtnIndex #radiobtnIndexTab #red #redrawViews #relativeRepeat #render #right #rotation #rotationX #rotationY #rotationZ #scale"
            "#scaleX #scaleY #scaleZ #scene #screen #selOnly #select #selectedObjects #selectedTracks #selection #shadeCverts #shaderColor #shaderTransparency #skipDups #smallCircle"
            "#smallDiamond #smallHollowBox #smallTriangle #sound #spacewarpBindings #specular #squash #staticValues #step #string #stringTab #summary #swapModel #system #text #texture"
            "#texturedBkg #texturemap #texturemapTab #time #timeChange #timeTab #transforms #triStrips #triangle #twoSided #txtCorrect #unClamped #universe #uscale #utility #velocity"
            "#vertTicks #vertical #videoPost #view #virtualVpts #visibilityTracks #visibleObjects #weight #wireframe #wireframeStrips #world #worldUnitsTab #worldunits #xMarker #zBuffer";

    if (set == 6)
        return
            "abs acos add addASec addAndWeld addAtmospheric addCam addEaseCurve addEffect addFocalNode addGlow addKnot addLight addLightNode addMSec addModToSelection addModifier"
            "addMorphTarget addMultiplerCurve addNURBSSet addNewKey addNewNoteKey addNewSpline addNewXRefFile addNewsXRefObject addNoteTrack addPluginRollouts addProperty addQuad"
            "addRay addRing addRollout addScript addStar addStreak addTrackViewController addTri affect_region allowBlending anchor animateAll animateVertex append appendCurve"
            "appendCurveByID appendGizmo appendObject appendSubSelSet appendUCurveByID appendVCurve appendVCurveByID applyEaseCurve applyOffset apropos arbAxis arc asin atan atan2"
            "attach attachMultiple attachObjects attemptUnresolvedXRefs audioClip audiofloat audiopoint3 audioposition audiorotation audioscale autoEdge averageSelVertCenter"
            "averageSelVertNormal awning axisTripodLocked"
            "background bend bevel bevel_profile bezier bezier_color bezier_float bezier_position bezier_rotation bezier_scale bifold billboard bindKnot bindSpaceWarp bitmap blizzard"
            "bomb bone boolObj.createBooleanObject box boxGizmo boxPickNode break breakCurve breakSurface broadcastCallback buildTVFaces buildVCFaces button"
            "c_ext camPoint camera_map canConvertTo cap_holes capsule casement ceil chamferbox chamfercyl checkForSave checkbox checkbutton circle circlePickNode classOf clear"
            "clearAllAppData clearCurSelSet clearEdgeVertices clearFilter clearListener clearNodeSelection clearProdTess clearScreen clearSubSelSets clearUndoBuffer clearViewTess"
            "close closeLog closeRolloutFloater closeU closeUtility closeV collapse collapseFace collapseStack colorpicker combobox compass completeRedraw composite cone"
            "configureBitmapPaths conformSpaceWarp conformToShape conjugate contains convertTo convertToMesh convertToNURBSCurve convertToNURBSSurface convertToRigid convertToSplineShape"
            "coordsys copy copyfile cos cosh count createBooleanObject createFile createInstance createLockKey createMorphObject createOLEObject createPreview createReaction"
            "createShapeFromEdges cross crossSection curveLength cycle cylGizmo cylinder"
            "damper defaultVCFaces deflector degToRad delete deleteAllChangeHandlers deleteAllXRefs deleteAppData deleteAtmosheric deleteCam deleteCamByName deleteChangeHandler"
            "deleteEaseCurve deleteEffect deleteElement deleteElementByName deleteFace deleteFile deleteFocalNode deleteFocalNodeByName deleteGizmo deleteItem deleteKey deleteKeys"
            "deleteKnot deleteLight deleteLightByName deleteMesh deleteModifier deleteMorphTarget deleteMultiplierCurve deleteNoteKey deleteNoteKeys deleteNoteTrack deleteObjects"
            "deleteProperty deleteReaction deleteSplie deleteTime deleteTrackViewController deleteTrackViewNode deleteVert dependents deselect deselectHiddenEdges deselectHiddenFaces"
            "deselectJey deselectKeys deselectNode detach directionalLight disableSceneRedraw disableStatusXYZ disconnect disp_approx displace displace_Mesh displace_NURBS display"
            "displayGamma displaySafeFrames displayTempPrompt distance divide donut dot dropdownlist dummy"
            "edit editAtmosphere editAtmospheric editEffect edit_mesh edit_patch edit_spline edittext elargeUpdateRect ellipse empty enableCoordCenter enableEscape enableORTs"
            "enablePixelSampler enableRefCoordSys enableSceneRedraw enableShowEndRes enableStatusXYZ enableSubObjSel enableUndo encryptFile encryptScript enumerateFiles eof evalPos"
            "evalTangent evalUTangents evalVTangents exclusionListDlg execute exitrude exp explode explodeGroup exportFile extrudeFace"
            "face_extrude fencePickNode fetchMaxFile ffdBox ffdCyl ffd_2x2x2 ffd_3x3x3 ffd_4x4x4 ffd_select fileIn fileInGamma fileOutGamma filePos fileProperties fileSaveAsMatLib"
            "fileSaveMatLib filenameFromPath fileopenMathLib fillet_chamfer filterString findItem findProperty findString findUnresolvedXrefs fixed flagChanged flagForeground flashNodes"
            "flex flipNormal float_expression float_list float_motion_capture floor flush flushLog fogHelper foliage forceCompleteRedraw format fractalNoise freeSceneBitmaps freeSpot"
            "freecamera freeze"
            "gc genClassID gengon geosphere get getActiveCamera getAfterORT getAppData getAtmospheric getBackGroundController getBefore getBipedExportInterface getBkgFrameNum"
            "getBkgImageAnim getBkgImageAspect getBkgORType getBkgRangeVal getBkgStartTime getBkgSyncFrame getBoolCutType getBoolOP getCPDisp getCPTM getCV getCVertModel getCamera"
            "getChannel getChannelAsMask getCommandPanelTaksMode getConfigFile getCoordCenter getCrossing getCurrentObject getCurrentSelection getCurve getCurveID getCurveStartPoint"
            "getDimensions getDir getDirectories getDisplacementMapping getDisplayResult getDriverString getEdge getEdgeSelection getEdgeVis getEffect getEulerMatAngleRatio"
            "getEulerQuatAngleRation getF"
            "getFace getFaceMatID getFaceNormal gravity grid groupBox"
            "hedra helix"
            "inline inlineHelper"
            "l_ext label lathe lattice line linear_float linear_position linear_rotation linear_scale link_control linked_xform listbox lod lodHelper lookat"
            "macroScript mapbutton materialByElement materialModifier materialbutton maxVersion melt meshSmooth mesh_select mirror modPanel.addModToSelection moise_point3 motor"
            "navInfo ngon noiseModifier noise_float noise_positi"
            "normalModifier normalize_spray notetrack"
            "oiltank omniLight optimize"
            "parray patch patchDeform pathDeform path_Follow pbomb pcloud pickbutton pivoted plane point point3_expression point3_list point3_motion_capture position_expression"
            "position_list position_motion_capture preserve prism progressBar projected protractor proxSensor prs push pyramid"
            "quadpatch"
            "radiobuttons railing rcmenu rectangle relax ringwave ripple rotation_list rotation_motion_capture"
            "scale_expression scale_list scale_motion_capture sdeflector section skew skin sliceModifier slider slidingDoor slidingWindow smooth snow sound spaceBend spaceDisplace"
            "spaceFFDBox spaceFFDCyl spaceNoise spaceRipple spaceSkew spaceStretch spaceTaper spaceTwist spaceWave spearator sphere sphereGizmo spherify spindle spinner splineSelect"
            "spray spring squeeze star stl_check stretch submenu substitute superspray surface surface_mapper surfdeform"
            "tape taper targetCamera targetDirectionalLight targetSoit targetobject teapot tesselate text timeSensor timer torus torus_knot touchSensor trim_extend tripatch tube twist"
            "uDeflector unwrap_UVW utility uvmap uvw_xform"
            "vertexPaint volumeselect vrml_vrbl"
            "wave wind"
            "xform xrefs.addNewXRefObject";

    return 0;
}

QString QsciLexerMAXScript::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case CommentLine:
        return tr("Comment line");

    case Number:
        return tr("Number");

    case Word:
        return tr("Word");

    case String:
        return tr("String");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case UnclosedString:
        return tr("Unclosed string");

    case Keyword:
        return tr("Keyword");

    case StandardOperator:
        return tr("Standard operator");

    case Attribute:
        return tr("Attribute");

    case StandardFunction:
        return tr("Standard function");

    case StandardPackage:
        return tr("Standard package");

    case StandardType:
        return tr("Standard type");

    case KeywordSet7:
        return tr("User defined");
    }

    return QString();
}

void QsciLexerMAXScript::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setAtElseProp();
    setAtBeginProp();
    setAtParenthProp();
}

bool QsciLexerMAXScript::foldComments() const
{
    return fold_comments;
}

bool QsciLexerMAXScript::foldCompact() const
{
    return fold_compact;
}

bool QsciLexerMAXScript::foldAtElse() const
{
    return fold_atelse;
}

bool QsciLexerMAXScript::foldAtBegin() const
{
    return fold_atbegin;
}

bool QsciLexerMAXScript::foldAtParenthesis() const
{
    return fold_atparenth;
}

void QsciLexerMAXScript::setFoldComments(bool fold)
{
    fold_comments = fold;
    setCommentProp();
}

void QsciLexerMAXScript::setFoldCompact(bool fold)
{
    fold_compact = fold;
    setCompactProp();
}

void QsciLexerMAXScript::setFoldAtElse(bool fold)
{
    fold_atelse = fold;
    setAtElseProp();
}

void QsciLexerMAXScript::setFoldAtBegin(bool fold)
{
    fold_atbegin = fold;
    setAtBeginProp();
}

void QsciLexerMAXScript::setFoldAtParenthesis(bool fold)
{
    fold_atparenth = fold;
    setAtParenthProp();
}

void QsciLexerMAXScript::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}

void QsciLexerMAXScript::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}

void QsciLexerMAXScript::setAtElseProp()
{
    emit propertyChanged("fold.at.else",(fold_atelse ? "1" : "0"));
}

void QsciLexerMAXScript::setAtBeginProp()
{
    emit propertyChanged("fold.at.begin",(fold_atbegin ? "1" : "0"));
}

void QsciLexerMAXScript::setAtParenthProp()
{
    emit propertyChanged("fold.at.Parenthese",(fold_atparenth ? "1" : "0"));
}

bool QsciLexerMAXScript::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", true).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_atelse = qs.value(prefix + "foldatelse", true).toBool();
    fold_atbegin = qs.value(prefix + "foldatbegin", true).toBool();
    fold_atparenth = qs.value(prefix + "foldatparenthesis", true).toBool();

    return rc;
}

bool QsciLexerMAXScript::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldatelse", fold_atelse);
    qs.setValue(prefix + "foldatbegin", fold_atbegin);
    qs.setValue(prefix + "foldatparenthesis", fold_atparenth);

    return rc;
}
