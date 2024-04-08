#pragma once
#include "samp.h"

class CInput {
public:
    enum {
        MAX_CLIENT_CMDS = 144,
        MAX_CMD_LENGTH = 32,
    };

    struct IDirect3DDevice9* m_pDevice;
    struct CDXUTDialog*      m_pGameUI;
    struct CDXUTEditBox*     m_pEditbox;
    CMDPROC           m_commandProc[MAX_CLIENT_CMDS];
    char              m_szCommandName[MAX_CLIENT_CMDS][MAX_CMD_LENGTH + 1];
    int               m_nCommandCount;
    BOOL              m_bEnabled;
    char              m_szInput[129];
    char              m_szRecallBufffer[10][129];
    char              m_szCurrentBuffer[129];
    int               m_nCurrentRecall;
    int               m_nTotalRecall;
    CMDPROC           m_pDefaultCommand;

    CInput(IDirect3DDevice9* pDevice);

    void    GetRect(struct CRect* pRect);
    void    Open();
    void    Close();
    void    AddRecall(const char* szString);
    void    RecallUp();
    void    RecallDown();
    void    EnableCursor();
    CMDPROC GetCommandHandler(const char* szName);
    void    SetDefaultCommand(CMDPROC handler);
    void    AddCommand(const char* szName, CMDPROC handler);
    BOOL    MsgProc(int uMsg, int wParam, int lParam);
    void    ResetDialogControls(CDXUTDialog* pGameUi);
    void    Send(const char* szString);
    void    ProcessInput();
};

CInput*& RefInputBox();