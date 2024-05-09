#include "MSWTerminalServices.h"
#include <wx/nativewin.h>
#include <wtsapi32.h> // terminal services session change notification

using namespace MSWTerminalServices;

wxDEFINE_EVENT(MSWTerminalServices::EVT_WTS_SESSION_CHANGE, SessionChangeEvent);

SessionChangeEvent::SessionChangeEvent(wxWindow* win, SessionChangeReason sessionChangeReason_, wxInt64 sessionID_) :
    sessionChangeReason(sessionChangeReason_), sessionID(sessionID_),
    wxEvent(win ? win->GetId() : 0, EVT_WTS_SESSION_CHANGE)
{}

RegisterForSessionChangeNotification::RegisterForSessionChangeNotification(wxWindow& win_) :
    win(win_)
{
   ::WTSRegisterSessionNotification(win.GetHWND(), NOTIFY_FOR_THIS_SESSION);
   win.MSWRegisterMessageHandler(WM_WTSSESSION_CHANGE, MSWWindowProc);
}

RegisterForSessionChangeNotification::~RegisterForSessionChangeNotification()
{
   ::WTSUnRegisterSessionNotification(win.GetHWND());
}

bool RegisterForSessionChangeNotification::MSWWindowProc(wxWindow* win, WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
   // invoke from client window's version of this routine
   if (WM_WTSSESSION_CHANGE != message)
      return false;

   // build and dispatch a wxEvent here
   
   const wxInt64 sessionID = lParam;
   switch (wParam)
   {
   case WTS_CONSOLE_CONNECT:
      // The session identified by lParam was connected to the console terminal or RemoteFX session.
      return HandleEvent(win, CONSOLE_CONNECT, sessionID);
      break;
   case WTS_CONSOLE_DISCONNECT:
      // The session identified by lParam was disconnected from the console terminal or RemoteFX session.
      return HandleEvent(win, CONSOLE_DISCONNECT, sessionID);
      break;
   case WTS_REMOTE_CONNECT:
      // The session identified by lParam was connected to the remote terminal.
      return HandleEvent(win, REMOTE_CONNECT, sessionID);
      break;
   case WTS_REMOTE_DISCONNECT:
      // The session identified by lParam was disconnected from the remote terminal.
      return HandleEvent(win, REMOTE_DISCONNECT, sessionID);
      break;
   case WTS_SESSION_LOGON:
      // A user has logged on To the session identified by lParam.
      return HandleEvent(win, SESSION_LOGON, sessionID);
      break;
   case WTS_SESSION_LOGOFF:
      // A user has logged off the session identified by lParam.
      return HandleEvent(win, SESSION_LOGOFF, sessionID);
      break;
   case WTS_SESSION_LOCK:
      // The session identified by lParam has been locked.
      return HandleEvent(win, SESSION_LOCK, sessionID);
      break;
   case WTS_SESSION_UNLOCK:
      // The session identified by lParam has been unlocked.
      return HandleEvent(win, SESSION_UNLOCK, sessionID);
      break;
   case WTS_SESSION_REMOTE_CONTROL:
      // The session identified by lParam has changed its remote controlled status. To determine the status, call GetSystemMetrics and check the SM_REMOTECONTROL metric.
      return HandleEvent(win, SESSION_REMOTE_CONTROL, sessionID);
      break;
   case WTS_SESSION_CREATE:
      // Reserved for future use.
      return HandleEvent(win, SESSION_CREATE, sessionID);
      break;
   case WTS_SESSION_TERMINATE:
      // Reserved for future use.
      return HandleEvent(win, SESSION_TERMINATE, sessionID);
      break;
   default:
      return false; 
   }
}

bool RegisterForSessionChangeNotification::HandleEvent(wxWindow* win, SessionChangeReason sessionChangeReason, wxInt64 sessionID)
{
#if 0 // debug output
   wxString msg("HandleEvent({type=");
   msg << GetSessionChangeReasonName(sessionChangeReason);
   msg << ",id=";
   msg << sessionID;
   msg << "})\n";
   OutputDebugStringA(msg.c_str());
#endif
   SessionChangeEvent event(win, sessionChangeReason, sessionID);
   return win->HandleWindowEvent(event);
}

wxEvent* SessionChangeEvent::Clone() const { return new SessionChangeEvent(*this); }

const char* MSWTerminalServices::ToString(SessionChangeReason sessionChangeReason)
{
   switch( sessionChangeReason)
   {
   case CONSOLE_CONNECT: return "CONSOLE_CONNECT";
   case CONSOLE_DISCONNECT: return "CONSOLE_DISCONNECT";
   case REMOTE_CONNECT: return "REMOTE_CONNECT";
   case REMOTE_DISCONNECT: return "REMOTE_DISCONNECT";
   case SESSION_LOGON: return "SESSION_LOGON";
   case SESSION_LOGOFF: return "SESSION_LOGOFF";
   case SESSION_LOCK: return "SESSION_LOCK";
   case SESSION_UNLOCK: return "SESSION_UNLOCK";
   case SESSION_REMOTE_CONTROL: return "SESSION_REMOTE_CONTROL";
   case SESSION_CREATE: return "SESSION_CREATE";
   case SESSION_TERMINATE: return "SESSION_TERMINATE";
   }
   return "unknown SessionChangeReason code";
}

#pragma comment(lib, "wtsapi32.lib")

DEFINE_EVENT_TYPE(SessionChangeEventType);

