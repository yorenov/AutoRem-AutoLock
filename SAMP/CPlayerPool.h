#pragma once
#include "samp.h"
#include "CLocalPlayer.h"
#include <string>

class CPed;
class CObject;

class CPlayerInfo {
public:
    struct CRemotePlayer* m_pPlayer;
    int            m_nPing;
#ifndef _DEBUG
private:
    int __aling;

public:
#endif
    std::string m_szNick;
    int         m_nScore;
    BOOL        m_bIsNPC;
};

class CPlayerPool {
public:
    enum { MAX_PLAYERS = 1004 };

    int          m_nLargestId;
    CPlayerInfo* m_pObject[MAX_PLAYERS];
    BOOL         m_bNotEmpty[MAX_PLAYERS];
    BOOL         m_bPrevCollisionFlag[MAX_PLAYERS];

    struct {
        int m_nPing;
        int m_nScore;
        ID  m_nId;
#ifndef _DEBUG
    private:
        int __align;

    public:
#endif
        std::string   m_szName;
        CLocalPlayer* m_pObject;
    } m_localInfo;

    CPlayerPool();
    ~CPlayerPool();

    void           UpdateLargestId();
    void           Process();
    ID             Find(::CPed* pGamePed);
    void           Deactivate();
    void           ForceCollision();
    void           RestoreCollision();
    BOOL           Delete(ID nId, int nReason = 0);
    BOOL           Create(ID nId, const char* szName, BOOL bIsNPC);
    struct CRemotePlayer* GetPlayer(ID nId);
    const char*    GetLocalPlayerName();
    BOOL           IsDisconnected(ID nId);
    BOOL           IsConnected(ID nId);
    void           SetLocalPlayerName(const char* szName);
    void           SetAt(ID nId, CPlayerInfo* pObject);
    int            GetScore(ID nId);
    int            GetPing(ID nId);
    const char*    GetName(ID nId);
    int            GetLocalPlayerPing();
    int            GetLocalPlayerScore();
    int            GetCount(BOOL bIncludeNPC = 0);
    CLocalPlayer*  GetLocalPlayer();
    CObject*       FindAccessory(::CObject* pGameObject);
};