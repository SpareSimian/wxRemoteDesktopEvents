#include "MSWTerminalServices.h"
#include <wx/nativewin.h>
#include <wtsapi32.h> // terminal services session change notification

using namespace MSWTerminalServices;

SessionChangeNotification::SessionChangeNotification(wxWindow& win_) :
    win(win_)
{
   ::WTSRegisterSessionNotification(win.GetHWND(), NOTIFY_FOR_THIS_SESSION);
   win.MSWRegisterMessageHandler(WM_WTSSESSION_CHANGE, MSWWindowProc);
}

SessionChangeNotification::~SessionChangeNotification()
{
   ::WTSUnRegisterSessionNotification(win.GetHWND());
}

bool SessionChangeNotification::MSWWindowProc(wxWindow* win, WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
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

bool SessionChangeNotification::HandleEvent(wxWindow* win, SessionChangeType sessionChangeType, wxInt64 sessionID)
{
   SessionChangeEvent event(sessionChangeType, sessionID);
   return win->HandleWindowEvent(event);
}

wxEvent* SessionChangeEvent::Clone() const { return new SessionChangeEvent(*this); }

#pragma comment(lib, "wtsapi32.lib")

DEFINE_EVENT_TYPE(SessionChangeEventType);
