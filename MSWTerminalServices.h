#pragma once

#ifdef __WXMSW__

#include <wtsapi32.h> // terminal services session change notification

class TerminalSessionChangeNotification
{
   HWND hwnd; // for deregistration
public:
   TerminalSessionChangeNotification(HWND hwnd);
   ~TerminalSessionChangeNotification();
   // invoke from client window's version of this routine
   void  MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

   // The session identified by lParam was connected to the console terminal or RemoteFX session.
   virtual void OnConsoleConnect(wxInt64 WXUNUSED(sessionID)) {}
   // The session identified by lParam was disconnected from the console terminal or RemoteFX session.
   virtual void OnConsoleDisconnect(wxInt64 WXUNUSED(sessionID)) {}
   // The session identified by lParam was connected to the remote terminal.
   virtual void OnRemoteConnect(wxInt64 WXUNUSED(sessionID)) {}
   // The session identified by lParam was disconnected from the remote terminal.
   virtual void OnRemoteDisconnect(wxInt64 WXUNUSED(sessionID)) {}
   // A user has logged on To the session identified by lParam.
   virtual void OnSessionLogon(wxInt64 WXUNUSED(sessionID)) {}
   // A user has logged off the session identified by lParam.
   virtual void OnSessionLogoff(wxInt64 WXUNUSED(sessionID)) {}
   // The session identified by lParam has been locked.
   virtual void OnSessionLock(wxInt64 WXUNUSED(sessionID)) {}
   // The session identified by lParam has been unlocked.
   virtual void OnSessionUnlock(wxInt64 WXUNUSED(sessionID)) {}
   // The session identified by lParam has changed its remote controlled status. To determine the status, call GetSystemMetrics and check the SM_REMOTECONTROL metric.
   virtual void OnSessionRemoteControl(wxInt64 WXUNUSED(sessionID)) {}
   // Reserved for future use.
   virtual void OnSessionCreate(wxInt64 WXUNUSED(sessionID)) {}
   // Reserved for future use.
   virtual void OnSessionTerminate(wxInt64 WXUNUSED(sessionID)) {}

};

TerminalSessionChangeNotification::TerminalSessionChangeNotification(HWND hwnd_) :
    hwnd(hwnd_)
{
   ::WTSRegisterSessionNotification(hwnd, NOTIFY_FOR_THIS_SESSION);
}

TerminalSessionChangeNotification::~TerminalSessionChangeNotification()
{
   ::WTSUnRegisterSessionNotification(hwnd);
}

void TerminalSessionChangeNotification::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
   if (WM_WTSSESSION_CHANGE == message)
   {
      const wxInt64 sessionID = lParam;
      switch (wParam)
      {
      case WTS_CONSOLE_CONNECT:
         // The session identified by lParam was connected to the
         // console terminal or RemoteFX session.
         OnConsoleConnect(sessionID);
         break;
      case WTS_CONSOLE_DISCONNECT:
         // The session identified by lParam was disconnected from the console terminal or RemoteFX session.
         OnConsoleDisconnect(sessionID);
         break;
      case WTS_REMOTE_CONNECT:
         // The session identified by lParam was connected to the remote terminal.
         OnRemoteConnect(sessionID);
         break;
      case WTS_REMOTE_DISCONNECT:
         // The session identified by lParam was disconnected from the remote terminal.
         OnRemoteDisconnect(sessionID);
         break;
      case WTS_SESSION_LOGON:
         // A user has logged on To the session identified by lParam.
         OnSessionLogon(sessionID);
         break;
      case WTS_SESSION_LOGOFF:
         // A user has logged off the session identified by lParam.
         OnSessionLogoff(sessionID);
         break;
      case WTS_SESSION_LOCK:
         // The session identified by lParam has been locked.
         OnSessionLock(sessionID);
         break;
      case WTS_SESSION_UNLOCK:
         // The session identified by lParam has been unlocked.
         OnSessionUnlock(sessionID);
         break;
      case WTS_SESSION_REMOTE_CONTROL:
         // The session identified by lParam has changed its remote controlled status. To determine the status, call GetSystemMetrics and check the SM_REMOTECONTROL metric.
         OnSessionRemoteControl(sessionID);
         break;
      case WTS_SESSION_CREATE:
         // Reserved for future use.
         OnSessionCreate(sessionID);
         break;
      case WTS_SESSION_TERMINATE:
         // Reserved for future use.
         OnSessionTerminate(sessionID);
         break;
      }
   }
}

#pragma comment(lib, "wtsapi32.lib")

#endif

