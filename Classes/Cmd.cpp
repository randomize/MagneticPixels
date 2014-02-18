#include "Cmd.h"

using namespace MPix;

// Only reserve memory

// UI views

unordered_map<string,function<ErrorCode(int,CmdUIUpdatePixelView::Reason)>> MPix::CmdUIUpdatePixelView::listners;
unordered_map<string,function<ErrorCode(int,MPix::Coordinates,CmdUIUpdateGoalView::Reason)>> MPix::CmdUIUpdateGoalView::listners;

unordered_map<string,function<ErrorCode(Point)>> MPix::CmdUIPutMark::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdUICreateViews::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdUIAddAssembly::listners;
unordered_map<string,function<ErrorCode(CmdUIRemoveAsembly::DestroyType)>> MPix::CmdUIRemoveAsembly::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdUIUpdateViewport::listners;
unordered_map<string,function<ErrorCode(int)>> MPix::CmdUIAddPixel::listners;
unordered_map<string,function<ErrorCode(int)>> MPix::CmdUIRemovePixel::listners;
unordered_map<string,function<ErrorCode(int,Coordinates)>> MPix::CmdUIMovePixel::listners;
unordered_map<string,function<ErrorCode(int,Direction)>> MPix::CmdUIHighlightPixel::listners;
unordered_map<string,function<ErrorCode(float,Point)>> MPix::CmdUIContentTransform::listners;

// UI main
unordered_map<string,function<ErrorCode()>> MPix::CmdUIGameFinished::listners;
unordered_map<string, function<ErrorCode(Node*)>> MPix::CmdUIShowNotification::listners;

// SoundManager commands
unordered_map<string,function<ErrorCode(const string&)>> MPix::CmdPlaySound::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdWillEnterFG::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdDidEnterBG::listners;

// GameplayManager commands
unordered_map<string,function<ErrorCode(void)>> MPix::CmdGameplayRestartAssembly::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdGameplayRedoMove::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdGameplayUndoMove::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdGameplayFirstMove::listners;
unordered_map<string,function<ErrorCode(Direction)>> MPix::CmdGameplayMove::listners;
unordered_map<string,function<ErrorCode(Direction)>> MPix::CmdGameplayAfterMove::listners;
unordered_map<string,function<ErrorCode(Coordinates)>> MPix::CmdGameplayClick::listners;
unordered_map<string,function<ErrorCode(void)>> MPix::CmdGameplayGrowAsm::listners;

// Editor commands
unordered_map<string,function<ErrorCode(CmdEditorAction::EditorAction)>> MPix::CmdEditorAction::listners;




