#pragma once

#include "samp.h"
#include "CVector.h"
#include "CPlayerPool.h"

struct Packet;
struct NetworkID;
class RakClientInterface;

enum MarkersMode {
    MARKERS_MODE_OFF,
    MARKERS_MODE_GLOBAL,
    MARKERS_MODE_STREAMED
};

class CNetGame {
public:
    enum GameMode {
        GAME_MODE_WAITCONNECT = 0x1,
        GAME_MODE_CONNECTING = 0x2,
        GAME_MODE_CONNECTED = 0x5,
        GAME_MODE_WAITJOIN = 0x6,
        GAME_MODE_RESTARTING = 0xB
    };
    enum {
        NETMODE_STATS_UPDATE_DELAY = 1000,
        NETMODE_INCAR_SENDRATE = 30,  // passenger/trailer/incar/unoccupied
        NETMODE_ONFOOT_SENDRATE = 30, // onfoot/unoccupied
        NETMODE_FIRING_SENDRATE = 30,
        LANMODE_INCAR_SENDRATE = 15,
        LANMODE_ONFOOT_SENDRATE = 15,
        NETMODE_SEND_MULTIPLIER = 2,
        NETMODE_AIM_UPDATE_DELAY = 500,
        NETMODE_HEAD_UPDATE_DELAY = 1000,
        NETMODE_TARGET_UPDATE_DELAY = 100,
        NETMODE_PLAYERS_UPDATE_DELAY = 3000,
    };

    struct Pools {
        struct CMenuPool*     m_pMenu;
        struct CActorPool*    m_pActor;
        CPlayerPool*   m_pPlayer;
        struct CVehiclePool*  m_pVehicle;
        struct CPickupPool*   m_pPickup;
        struct CObjectPool*   m_pObject;
        struct CGangZonePool* m_pGangZone;
        struct CLabelPool*    m_pLabel;
        struct CTextDrawPool* m_pTextDraw;
    };

    struct Settings {
        bool          m_bUseCJWalk;
        unsigned int  m_nDeadDropsMoney;
        float         m_fWorldBoundaries[4];
        bool          m_bAllowWeapons;
        float         m_fGravity;
        bool          m_bEnterExit;
        BOOL          m_bVehicleFriendlyFire;
        bool          m_bHoldTime;
        bool          m_bInstagib;
        bool          m_bZoneNames;
        bool          m_bFriendlyFire;
        BOOL          m_bClassesAvailable;
        float         m_fNameTagsDrawDist;
        bool          m_bManualVehicleEngineAndLight;
        unsigned char m_nWorldTimeHour;
        unsigned char m_nWorldTimeMinute;
        unsigned char m_nWeather;
        bool          m_bNoNametagsBehindWalls;
        int           m_nPlayerMarkersMode;
        float         m_fChatRadius;
        bool          m_bNameTags;
        bool          m_bLtdChatRadius;
    };

    char                pad_0[44];
    RakClientInterface* m_pRakClient;
    char                m_szHostAddress[257];
    char                m_szHostname[257];
    bool                m_bDisableCollision;
    bool                m_bUpdateCameraTarget;
    bool                m_bNametagStatus;
    int                 m_nPort;
    BOOL                m_bLanMode;
    GTAREF              m_aMapIcons[100];
    int                 m_nGameState;
    TICK                m_lastConnectAttempt;
    Settings*           m_pSettings;
    char                pad_2[5];
    Pools*              m_pPools;

    static TICK& RefLastPlayersUpdateRequest();

    CNetGame(const char* szHostAddress, int nPort, const char* szNick, const char* szPass);
    ~CNetGame();

    CPickupPool*        GetPickupPool();
    CMenuPool*          GetMenuPool();
    void                SetState(int nValue);
    void                InitializePools();
    void                InitialNotification();
    void                InitializeGameLogic();
    void                Connect();
    void                SpawnScreen();
    void                Packet_RSAPublicKeyMismatch(Packet* pPacket);
    void                Packet_ConnectionBanned(Packet* pPacket);
    void                Packet_ConnectionRequestAcepted(Packet* pPacket);
    void                Packet_NoFreeIncomingConnections(Packet* pPacket);
    void                Packet_DisconnectionNotification(Packet* pPacket);
    void                Packet_InvalidPassword(Packet* pPacket);
    void                Packet_ConnectionAttemptFailed(Packet* pPacket);
    void                UpdatePlayers();
    void                DeleteMarker(NUMBER nIndex);
    void                ResetPlayerPool();
    void                ResetVehiclePool();
    void                ResetTextDrawPool();
    void                ResetObjectPool();
    void                ResetGangZonePool();
    void                ResetPickupPool();
    void                ResetMenuPool();
    void                ResetLabelPool();
    void                ResetActorPool();
    void                Packet_UnoccupiedSync(Packet* pPacket);
    void                Packet_BulletSync(Packet* pPacket);
    void                Packet_AimSync(Packet* pPacket);
    void                Packet_PassengerSync(Packet* pPacket);
    void                Packet_TrailerSync(Packet* pPacket);
    void                Packet_MarkersSync(Packet* pPacket);
    void                Packet_AuthKey(Packet* pPacket);
    void                ResetMarkers();
    void                CreateMarker(NUMBER nIndex, CVector position, char nIcon, int nColor, int nType);
    void                ResetPools();
    void                ShutdownForRestart();
    void                Packet_PlayerSync(Packet* pPacket);
    void                Packet_VehicleSync(Packet* pPacket);
    void                Packet_ConnectionLost(Packet* pPacket);
    void                Packet_ConnectionSucceeded(Packet* pPacket);
    void                UpdateNetwork();
    void                Process();
    void                ProcessGameStuff();
    CPlayerPool*        GetPlayerPool();
    CObjectPool*        GetObjectPool();
    CActorPool*         GetActorPool();
    int                 GetState();
    BOOL                LanMode();
    CVehiclePool*       GetVehiclePool();
    RakClientInterface* GetRakClient();
    __int64             GetCounter();
};

CNetGame*& RefNetGame();