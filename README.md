# wxRemoteDesktopEvents

Allow a wxWidgets window to receive notifications when Remote Desktop
sessions are connected and disconnected. One application is to
automatically resize a window based on the new display size.

Usage is illustrated using the wxWidgets minimal sample. Simply
include the notification registration object in your window object and
initialize it with a reference to the window and Bind() the event to a
handler method:
```
    Bind(wts::EVT_WTS_SESSION_CHANGE, &MyFrame::OnSessionChange, this);
```

Session change API: https://learn.microsoft.com/en-us/windows/win32/termserv/wm-wtssession-change

Known issues:

WTSUnRegisterSessionNotication returns success but issues the following message to the debugger output window:
```
Exception thrown at 0x00007FF8C097AB89 (KernelBase.dll) in minimal.exe: 0x0000071A: The remote procedure call was canceled, or if a call time-out was specified, the call timed out.
```
