#ifndef _WX_DRAWMENUDIALOG_H_
#define _WX_DRAWMENUDIALOG_H_

#include <wx/wx.h>

class DrawMenuDialog : public wxDialog
{
public:
  DrawMenuDialog(const wxString& title);
  ~DrawMenuDialog();
  DrawMenuDialog(const DrawMenuDialog& source);

  void OnAccept(wxCommandEvent& event);
  void OnClose(wxCommandEvent& event);
  int getSelected() {return selected;}

private:
	int selected;

};

#endif // _WX_DRAWMENUDIALOG_H_