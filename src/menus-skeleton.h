#ifndef SEEN_MENUS_SKELETON_H
#define SEEN_MENUS_SKELETON_H

#include <inkscape_version.h>

#ifdef __cplusplus
#undef N_
#define N_(x) x
#endif

static char const menus_skeleton[] =
"<inkscape version=\"" INKSCAPE_VERSION "\"\n"
"  xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
"  xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\">\n"
"\n"
"   <submenu name=\"" N_("_File") "\">\n"
"       <submenu name=\"" N_("_New") "\">\n"
"           <verb verb-id=\"FileNew\" />\n"
"           <template-list/>\n"
"       </submenu>\n"
"       <verb verb-id=\"FileOpen\" />\n"
"       <submenu name=\"" N_("Open _Recent") "\">\n"
"           <recent-file-list/>\n"
"       </submenu>\n"
"       <verb verb-id=\"FileRevert\" />\n"
"       <verb verb-id=\"FileSave\" />\n"
"       <verb verb-id=\"FileSaveAs\" />\n"
"       <verb verb-id=\"FileSaveACopy\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"FileImport\" />\n"
"       <verb verb-id=\"FileExport\" />\n"
#ifdef WITH_GNOME_VFS
"       <verb verb-id=\"FileImportFromOCAL\" />\n"
"       <verb verb-id=\"FileExportToOCAL\" />\n"
#endif
"       <separator/>\n"
/* These are ugly, but what needs to happen here is allowing users
   to use the native PS support if they are using another print driver.
   This is done through the "Print Direct" command.  Which is inserted
   here based on if those other drivers are being built. */
#ifdef WIN32
"       <verb verb-id=\"FilePrintDirect\" />\n"
#endif
"       <verb verb-id=\"FilePrint\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"FileVacuum\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"DialogDocumentProperties\" />\n"
"       <verb verb-id=\"DialogMetadata\" />\n"
"       <verb verb-id=\"DialogPreferences\" />\n"
"       <verb verb-id=\"DialogInput\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"FileClose\" />\n"
"       <verb verb-id=\"FileQuit\" />\n"
"   </submenu>\n"
"   <submenu name=\"" N_("_Edit") "\">\n"
"       <verb verb-id=\"EditUndo\" />\n"
"       <verb verb-id=\"EditRedo\" />\n"
"       <verb verb-id=\"DialogUndoHistory\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"EditCut\" />\n"
"       <verb verb-id=\"EditCopy\" />\n"
"       <verb verb-id=\"EditPaste\" />\n"
"       <verb verb-id=\"EditPasteInPlace\" />\n"
"       <verb verb-id=\"EditPasteStyle\" />\n"
"       <verb verb-id=\"EditPasteLivePathEffect\" />\n"
"       <submenu name=\"" N_("Paste Si_ze") "\">\n"
"           <verb verb-id=\"EditPasteSize\" />\n"
"           <verb verb-id=\"EditPasteWidth\" />\n"
"           <verb verb-id=\"EditPasteHeight\" />\n"
"           <verb verb-id=\"EditPasteSizeSeparately\" />\n"
"           <verb verb-id=\"EditPasteWidthSeparately\" />\n"
"           <verb verb-id=\"EditPasteHeightSeparately\" />\n"
"       </submenu>\n"
"       <separator/>\n"
"       <verb verb-id=\"DialogFind\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"EditDuplicate\" />\n"
"       <submenu name=\"" N_("Clo_ne") "\">\n"
"           <verb verb-id=\"EditClone\" />\n"
"           <verb verb-id=\"DialogClonetiler\" />\n"
"           <verb verb-id=\"EditUnlinkClone\" />\n"
"           <verb verb-id=\"EditCloneOriginal\" />\n"
"       </submenu>\n"
"       <verb verb-id=\"SelectionCreateBitmap\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"EditDelete\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"EditSelectAll\" />\n"
"       <verb verb-id=\"EditSelectAllInAllLayers\" />\n"
"       <verb verb-id=\"EditInvert\" />\n"
"       <verb verb-id=\"EditDeselect\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"DialogXMLEditor\" />\n"
"   </submenu>\n"
"   <submenu name=\"" N_("_View") "\">\n"
"     <submenu name=\"" N_("_Zoom") "\">\n"
"       <verb verb-id=\"ZoomIn\" />\n"
"       <verb verb-id=\"ZoomOut\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"Zoom1:0\" />\n"
"       <verb verb-id=\"Zoom1:2\" />\n"
"       <verb verb-id=\"Zoom2:1\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"ZoomSelection\" />\n"
"       <verb verb-id=\"ZoomDrawing\" />\n"
"       <verb verb-id=\"ZoomPage\" />\n"
"       <verb verb-id=\"ZoomPageWidth\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"ZoomPrev\" />\n"
"       <verb verb-id=\"ZoomNext\" />\n"
"     </submenu>\n"
"       <submenu name=\"" N_("_Display mode") "\">\n"
"         <verb verb-id=\"ViewModeToggle\"/>\n"
"         <verb verb-id=\"ViewModeNormal\" radio=\"yes\" default=\"yes\"/>\n"
"         <verb verb-id=\"ViewModeOutline\" radio=\"yes\"/>\n"
"       </submenu>\n"
"       <separator/>\n"
"       <verb verb-id=\"ToggleGrid\" />\n"
"       <verb verb-id=\"ToggleGuides\" />\n"
"       <separator/>\n"
"       <submenu name=\"" N_("Show/Hide") "\">\n"
"           <objects-checkboxes/>\n"
"       </submenu>\n"
"       <verb verb-id=\"DialogsToggle\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"DialogSwatches\" />\n"
"       <verb verb-id=\"DialogDebug\" />\n"
"       <verb verb-id=\"DialogScript\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"PrevWindow\" />\n"
"       <verb verb-id=\"NextWindow\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"ViewIconPreview\" />\n"
"       <verb verb-id=\"ViewNew\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"FullScreen\" />\n"
"   </submenu>\n"
"   <submenu name=\"" N_("_Layer") "\">\n"
"       <verb verb-id=\"LayerNew\" />\n"
"       <verb verb-id=\"LayerRename\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"LayerNext\" />\n"
"       <verb verb-id=\"LayerPrev\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"LayerMoveToNext\" />\n"
"       <verb verb-id=\"LayerMoveToPrev\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"LayerRaise\" />\n"
"       <verb verb-id=\"LayerLower\" />\n"
"       <verb verb-id=\"LayerToTop\" />\n"
"       <verb verb-id=\"LayerToBottom\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"LayerDelete\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"DialogLayers\" />\n"
"   </submenu>\n"
"   <submenu name=\"" N_("_Object") "\">\n"
"       <verb verb-id=\"DialogFillStroke\" />\n"
"       <verb verb-id=\"DialogFilterEffects\" />\n"
"       <verb verb-id=\"DialogObjectProperties\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionGroup\" />\n"
"       <verb verb-id=\"SelectionUnGroup\" />\n"
"       <separator/>\n"
"       <submenu name=\"" N_("Cli_p") "\">\n"
"           <verb verb-id=\"ObjectSetClipPath\" />\n"
"           <verb verb-id=\"ObjectUnSetClipPath\" />\n"
"       </submenu>\n"
"       <submenu name=\"" N_("Mas_k") "\">\n"
"           <verb verb-id=\"ObjectSetMask\" />\n"
"           <verb verb-id=\"ObjectUnSetMask\" />\n"
"       </submenu>\n"
"       <submenu name=\"" N_("Patter_n") "\">\n"
"           <verb verb-id=\"ObjectsToPattern\" />\n"
"           <verb verb-id=\"ObjectsFromPattern\" />\n"
"       </submenu>\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionRaise\" />\n"
"       <verb verb-id=\"SelectionLower\" />\n"
"       <verb verb-id=\"SelectionToFront\" />\n"
"       <verb verb-id=\"SelectionToBack\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"ObjectRotate90\" />\n"
"       <verb verb-id=\"ObjectRotate90CCW\" />\n"
"       <verb verb-id=\"ObjectFlipHorizontally\" />\n"
"       <verb verb-id=\"ObjectFlipVertically\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"UnhideAll\" />\n"
"       <verb verb-id=\"UnlockAll\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"DialogTransform\" />\n"
"       <verb verb-id=\"DialogAlignDistribute\" />\n"
"       <verb verb-id=\"DialogGridArrange\" />\n"
"   </submenu>\n"
"   <submenu name=\"" N_("_Path") "\">\n"
"       <verb verb-id=\"ObjectToPath\" />\n"
"       <verb verb-id=\"StrokeToPath\" />\n"
"       <verb verb-id=\"SelectionTrace\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionUnion\" />\n"
"       <verb verb-id=\"SelectionDiff\" />\n"
"       <verb verb-id=\"SelectionIntersect\" />\n"
"       <verb verb-id=\"SelectionSymDiff\" />\n"
"       <verb verb-id=\"SelectionDivide\" />\n"
"       <verb verb-id=\"SelectionCutPath\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionCombine\" />\n"
"       <verb verb-id=\"SelectionBreakApart\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionInset\" />\n"
"       <verb verb-id=\"SelectionOffset\" />\n"
"       <verb verb-id=\"SelectionDynOffset\" />\n"
"       <verb verb-id=\"SelectionLinkedOffset\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionSimplify\" />\n"
"       <verb verb-id=\"SelectionReverse\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"DialogLivePathEffect\" />\n"
"   </submenu>\n"
"   <submenu name=\"" N_("_Text") "\">\n"
"       <verb verb-id=\"DialogText\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionTextFromPath\" />\n"
"       <verb verb-id=\"SelectionTextToPath\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"ObjectFlowText\" />\n"
"       <verb verb-id=\"ObjectUnFlowText\" />\n"
"       <verb verb-id=\"ObjectFlowtextToText\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"SelectionTextRemoveKerns\" />\n"
"   </submenu>\n"
"   <submenu name=\"" N_("Effe_cts") "\">\n"
"       <verb verb-id=\"EffectLast\" />\n"
"       <verb verb-id=\"EffectLastPref\" />\n"
"       <separator/>\n"
"       <effects-list/>\n"
"   </submenu>\n"
#ifdef WITH_INKBOARD
"	<submenu name=\"" N_("Whiteboa_rd") "\">\n"
"		<verb verb-id=\"DialogXmppClient\" />\n"
"	</submenu>\n"
#endif
"   <submenu name=\"" N_("_Help") "\">\n"
"       <verb verb-id=\"org.inkscape.help.manual\" />\n"
"       <verb verb-id=\"org.inkscape.help.keys\" />\n"
"       <submenu name=\"" N_("Tutorials") "\">\n"
"           <verb verb-id=\"TutorialsBasic\" />\n"
"           <verb verb-id=\"TutorialsShapes\" />\n"
"           <verb verb-id=\"TutorialsAdvanced\" />\n"
"           <verb verb-id=\"TutorialsTracing\" />\n"
"           <verb verb-id=\"TutorialsCalligraphy\" />\n"
"           <verb verb-id=\"TutorialsDesign\" />\n"
"           <verb verb-id=\"TutorialsTips\" />\n"
"       </submenu>\n"
//"       <verb verb-id=\"org.inkscape.dialogs.extensioneditor\" />\n"
"       <verb verb-id=\"org.inkscape.help.commandline\" />\n"
"       <verb verb-id=\"org.inkscape.help.faq\" />\n"
"       <verb verb-id=\"org.inkscape.help.relnotes\" />\n"
"       <verb verb-id=\"org.inkscape.help.reportabug\" />\n"
"       <verb verb-id=\"org.inkscape.help.svgspec\" />\n"
"       <separator/>\n"
"       <verb verb-id=\"HelpAboutMemory\" />\n"
"       <verb verb-id=\"HelpAbout\" />\n"
//"       <verb verb-id=\"ShowLicense\" />\n"
"   </submenu>\n"
"</inkscape>\n";

#define MENUS_SKELETON_SIZE (sizeof(menus_skeleton) - 1)


#endif /* !SEEN_MENUS_SKELETON_H */

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=99 :
