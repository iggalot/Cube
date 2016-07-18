#include "../include/DrawMenuDialog.h"

DrawMenuDialog::DrawMenuDialog(const wxString & title)
       : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230))
{

  wxPanel *panel = new wxPanel(this, -1);

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

  wxStaticBox *st = new wxStaticBox(panel, -1, wxT("Select Shape"), 
      wxPoint(5, 5), wxSize(240, 150));
  wxRadioButton *rb = new wxRadioButton(panel, -1, 
      wxT("256 Colors"), wxPoint(15, 30), wxDefaultSize, wxRB_GROUP);
  wxRadioButton *rb3 = new wxRadioButton(panel, -1, 
      wxT("Custom"), wxPoint(15, 105));
  wxTextCtrl *tc = new wxTextCtrl(panel, -1, wxT(""), 
      wxPoint(95, 105));

  wxButton *okButton = new wxButton(this, wxID_OK, wxT("Ok"), 
      wxDefaultPosition, wxSize(70, 30));
  wxButton *closeButton = new wxButton(this, wxID_CANCEL, wxT("Close"), 
      wxDefaultPosition, wxSize(70, 30));

  hbox->Add(okButton, 1);
  hbox->Add(closeButton, 1, wxLEFT, 5);

  vbox->Add(panel, 1);
  vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

  SetSizer(vbox);

  Centre();

  // Connect(wxID_OK, wxEVT_COMMAND_MENU_SELECTED,
  //   wxCommandEventHandler(DrawMenuDialog::OnAccept)); 
  // Connect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED,
  //   wxCommandEventHandler(DrawMenuDialog::OnClose)); 

  int result = ShowModal();
  std::cout << "ShowModal result: "  << result << " " << std::endl;
  std::cout << "wxID_OK result: "  << wxID_OK << " " << std::endl;
  std::cout << "wxID_CLOSE result: "  << wxID_CLOSE << " " << std::endl;
  std::cout << "wxID_CANCEL result: "  << wxID_CANCEL << " " << std::endl;

  if (result == wxID_OK)
    std::cout << "OK" << std::endl;
  else if (result == wxID_CANCEL)
    std::cout << "CANCELLED" << std::endl;
  else
    std::cout << "Another option selected" << std::endl;

  selected = result;

  //Destroy(); 
}

//copy constructor
DrawMenuDialog::DrawMenuDialog(const DrawMenuDialog &source) {
  selected = source.selected;
}

// destructor
DrawMenuDialog::~DrawMenuDialog() {
  std::cout << "Destrying DrawMenuDialog" << std::cout;
  Destroy();
}

// void DrawMenuDialog::OnAccept(wxCommandEvent& WXUNUSED(event))
// {
//     // true is to force the frame to close
//     Close(true);
// }

// void DrawMenuDialog::OnClose(wxCommandEvent& WXUNUSED(event))
// {
//     // true is to force the frame to close
//     Close(true);
// }