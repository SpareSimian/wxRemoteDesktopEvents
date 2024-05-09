#pragma once

#ifdef __WXMSW__

#include <wx/event.h>

namespace MSWTerminalServices {
   
enum SessionChangeReason
{
      // The session identified by lParam was connected to the console terminal or RemoteFX session.
   CONSOLE_CONNECT,
      // The session identified by lParam was disconnected from the console terminal or RemoteFX session.
   CONSOLE_DISCONNECT,
      // The session identified by lParam was connected to the remote terminal.
   REMOTE_CONNECT,
      // The session identified by lParam was disconnected from the remote terminal.
   REMOTE_DISCONNECT,
      // A user has logged on To the session identified by lParam.
   SESSION_LOGON,
      // A user has logged off the session identified by lParam.
   SESSION_LOGOFF,
      // The session identified by lParam has been locked.
   SESSION_LOCK,
      // The session identified by lParam has been unlocked.
   SESSION_UNLOCK,
      // The session identified by lParam has changed its remote controlled status. To determine the status, call GetSystemMetrics and check the SM_REMOTECONTROL metric.
   SESSION_REMOTE_CONTROL,
      // Reserved for future use.
   SESSION_CREATE,
      // Reserved for future use.
   SESSION_TERMINATE
};

const char* ToString(SessionChangeReason sessionChangeReason);

class SessionChangeEvent : public wxEvent
{
public:
   SessionChangeEvent::SessionChangeEvent(wxWindow* win, SessionChangeReason sessionChangeReason_, wxInt64 sessionID_);
   wxEvent* Clone() const override;

   SessionChangeReason sessionChangeReason;
   wxInt64 sessionID;
};

extern const wxEventType SessionChangeEventType;
wxDECLARE_EVENT(EVT_WTS_SESSION_CHANGE, SessionChangeEvent);

typedef void (wxEvtHandler::*SessionChangeEventFunction)(SessionChangeEvent &);

#define EVT_SESSION_CHANGE(fn) \
    DECLARE_EVENT_TABLE_ENTRY( SessionChangeEventType, wxID_ANY, wxID_ANY, \
                               (wxObjectEventFunction)(wxEventFunction) \
                               wxStaticCastEvent(SessionChangeEventFunction, &fn), \
                               (wxObject*) NULL  ),

class RegisterForSessionChangeNotification
{
   wxWindow& win; // for event dispatch and deregistration
public:
   RegisterForSessionChangeNotification(wxWindow& win);
   ~RegisterForSessionChangeNotification();
   private:
      static bool MSWWindowProc(wxWindow* win, WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
      static bool HandleEvent(wxWindow* win, SessionChangeReason sessionChangeReason, wxInt64 sessionID);

   wxDECLARE_NO_COPY_CLASS(RegisterForSessionChangeNotification);
};

}

#endif

