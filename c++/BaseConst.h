#pragma once
#include "BaseAsset.h"
#include <fstream>

// include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
namespace Logic{

	#ifndef baseconst
	#define baseconst
	template <class T>
	class PointerGuard{
	public:
		PointerGuard(){
		PointerGuard<T>::sigleton = this;
		
		}
		~PointerGuard(){
		PointerGuard::sigleton= 0;		
		}
		static PointerGuard<T>* GetSigleton(){
		return PointerGuard<T>::sigleton;
		
		}
		bool Get(T* pnt){
			if(mMap.count(pnt)>0){
				return true	;	
			}
			return false;
		
		}
		void Insert(T*pnt){
			mMap.insert(pnt);
		}
		void Remove(T*pnt){
			mMap.erase(pnt);		
		}
	protected:
		std::unordered_set<T*> mMap;
		static PointerGuard<T>*sigleton;
	};
	template <class T> PointerGuard<T> * PointerGuard<T>::sigleton =0;
	enum DateChageType{
		DC_NONE,
		DC_DAYS,
		DC_MONTH,
		DC_YEAR,
		DC_WEEK
	};
	enum EventType{
		ET_TIME,
		ET_GAME,
		ET_PLANET,
		ET_CONTROL,
		ET_GUI
	};
	enum ClickHelperType{
		CHT_BUILDING,
		CHT_EMPTYSPACE,
		CHT_COUNTRY,
	};
	enum ControlTypeEvent{
		CTE_SHOWRESOURCE,
		CTE_SHOWBUILDING,
		CTE_ASKFORBLUEPRINT,
		CTE_BUILDSOMETHING,
		CTE_ASKFORCOUNTRY,
		CTE_BUILDCOREINCOUNTRY,
		CTE_SETDEPOSITE,
		CTE_SETRESEARCH,
		CTE_ASKDETAILEDRESOURSE,
		CTE_ASKDETAILEDRESOURSERESEARCH,
		CTE_USERESOURCEONRESEARCH,
		CTE_SHOWREQUESTS,
		CTE_RESOURSEREDIALOG,
		CTE_RESOURSEREDIALOGCHANGE,
		CTE_RESOURSEREDIALOGSEND,
		CTE_RESOURSEREDIALOGDECLINE,
		CTE_DIALOGFORACTION,
		CTE_DIALOGFORACTIONGSEND,
		CTE_DIALOGFORACTIONDECLINE,
		CTE_DIALOGFORACTIONSETSOMTHING,
		CTE_ADDUNITTOQUEUE,
		CTE_ASKFORRESEARCH,
		CTE_ACCEPTPROJECT,
		CTE_DECLINEPROJECT,
		CTE_OPENPROJECTDIALOG,
		CTE_RESOURCEPROJDIALOGCHANGE,
		CTE_RESOURSEPROJDIALOGSEND,
		CTE_RESOURSEPROJDIALOGDECLINE,
		CTE_DELETEBUILDING,
		CTE_ASKTRADE,
		CTE_SETPRODUCTION,
		CTE_UNITCLICK,
		CTE_SENDUNIT,
		CTE_SETWARFAREACTION,
		CTE_SHOWWARAFERE,
		CTE_SHOWRECON,
		CTE_SETRECON,
		CTE_PAUSEGAME,
		CTE_RESUMEGAME,
		CTE_SAVEGAME,
		CTE_ACTIVATEABILITY
	};
	enum GameEventType{
		ET_SOMEDESTRACT,
		ET_NOTIFY,
		GUIET_SHOWRESOURSE,
		GUIET_BUILDINGDUILT,
		GUIET_COREBUILT,
		GUIET_SENDCORETOGUI,
		GUIET_SENDBLUETOGUI,
		GUIET_PLANETFINISHGENERATION,
		GUIET_COUNTRYDATA,
		GUIET_SHOWMINING,
		GUIET_SHOWPRODUCTION,
		GUIET_SHOWWAREHOUSE,
		GUIET_SHOWRESEARCHCENTER,
		GUIET_SHOWALLRESOURSE,
		GUIET_SHOWALLRESOURSERES,
		GUIET_SHOWINTELEGANCE,
		GUIET_SHOWMILITARYBASE,
		GUIET_SHOWALLREQUEST,
		GUIET_SHOWREQRESDIALOG,
		GUIET_CLOSEREQRESDIALOG,
		GUIET_ADDNEWNEWS,
		GUIET_SHOWACTINTDIALOG,
		GUIET_CLOSEACTINTDIALOG,
		GUIET_SHOWPROJDIALOG,
		GUIET_CLOSEPROJRESDIALOG,
		GUIET_SHOWUNITINBASE,
		GUIET_CLOSEUNITDIALOG,
		GUIET_SHOWWARFARE,
		GUIET_CLOSEWARFARE,
		GUIET_SHOWRECON,
		GUIET_RESETCAM,
	};
	enum NotificationType{
		NTE_BUILDINGIDLE,
		NTE_REQUESTEEXPIRATE,
		NTE_RESEARCHCOMPLITE,
		NTE_EMBASSYCOMPLITE,
		NTE_PROJECTFAILUREUSERBY,
		NTE_PROJECTFAILURE,
		NTE_PROJECTSUCCSESS,
		NTE_BATTLEOVER,
	};
	enum ReseourseOpenFrom{
		ROF_SIMPLESHOW,
		ROF_RESEARCH
	};
	enum GeneratorType{
		GENT_SIMPLE
	};
	const int AX_GENERATOT =10;

	
	enum TYPE_OF_BUILDING{
		TB_BASE,
		TB_BASECORE,
		TB_WAREHOUSE,
		TB_MINING,
		TB_PRODUCTION,
		TB_RESEARCH,
		TB_MILITARY,
		TB_EMBASSY,
		TB_BLACKOPS,
		TB_ORBMINING,

	};
	enum BUILDING_PLACE_TYPE{
		BPT_GROUD,
		BPT_GROUND_ORBITAL,
		BPT_ORBITAL
	};
	enum SOME_CHANGE_TYPE{
		SCT_INCRESEDEPODIT,
		SCT_WARHEOUSESPACEINCREASE,
		SCT_UNITINCREASE,
	};
	enum TYPE_OF_PRODUCTION{
		TYPROD_REFINERY,
		TYPROD_PRODUCTION
	};
	enum TimeFormat{
		 DF_SIMPLE,
		 DF_INT,
		 DF_DAYS,
		 DF_NORMAL
	};
	enum GameState{
		NOTSTART,
		START,
		PREFINISH,
		FINISH,
		OVER
	};
	
	enum ActonType{
		AT_BLACKOPS,
		AT_DIMPLOMATS,
		AT_RECON

	};
	

	enum CharacteristicType{
		CT_INT,
		CT_BOOL,
		CT_SIGNINT,
		CT_DECS100,
		CT_DECS100RN,
		CT_DESCMANY,
		CT_DESCREMENT,
		CT_DELTA

	};
	enum CountryRelationStatus{
		CRS_NEUTRAL,
		CRS_TRADE,
		CRS_WAR,
		CRS_ALLIANCE,
		CRS_COLDWAR
	};
	enum PlanetType{
		PT_NORMAL,
		PT_BUGS,
		PT_PLANTS
	};
	enum PlanetEnvorementType{
		PET_NORMAL,
		PET_GAS
	};
	enum PlanetEventType{
		PET_SIMPLE
	};
	
	
	enum ResourceType{
		RT_ORES,
		RT_PLANETPRODUCTION,
		RT_PRODUCTION,
		RT_INT
	
	};
	enum AbilityType{
		ABILT_PLANET,
		ABILT_CORP,
		ABILT_BUILDING,
		ABILT_BATTLE,
		ABILT_UNIT,
		ABILT_CORE,
		ABILT_DEPOSIT,
		ABILT_COUNTRY,
		ABILT_EMBASSY,
		ABILT_MINING,
	};
	enum ShowCharType{
		SCT_NONE,
		SCT_SIMPLE,
		
	};
	enum EffectType{
		EFT_ALLPRODUCTION,
		EFT_MINNING,
	};
	enum ChangeEffectType{
		CEFT_SUM,
		CEFT_MULTIPLIE
	};
	struct BaseEvent
	{
		BaseEvent(){
		isReSend= false;
		}
		EventType type;
		bool isReSend;
	};

	struct GameEvent : public BaseEvent{
		GameEvent() 
		{
			gameType =ET_SOMEDESTRACT;
			type =ET_GAME;
		}
		virtual ~GameEvent(){};
		GameEventType gameType;
	};
	struct NotificationEvent : public GameEvent{
		NotificationEvent() 
		{
			gameType= ET_NOTIFY;
			notType =NTE_BUILDINGIDLE;
			type =ET_GAME;
		}
		NotificationType notType;
		void *target;
		Ogre::String name;
	};
	enum BuildingWorkMode{
		BWM_NORMAL,
		BMW_TWICE,
	};
	enum ProjectPeriodType{
		PTP_WEEKLY,
		PTP_MONTHLY,
	};
	enum ProjectCallBackType{
		PCBT_ADDRESEARCHPOINT,
	};
	enum ActionCallBackType{
		ACBT_FISRCTCONTACT,
		ACBT_ABDUCTION
	};
	enum WarfareAction{
		WARA_STAND,
		WARA_OCUPIE,
		WARA_PILLAGE,
		WARA_KILLTHEMALL,
		WARA_CUT_SUPPLIES,
		WARA_RESTORE_SUPPLIES,
		WARA_EVACUATE,
		
	};
	enum WarState{
		WARSTS_NONE,
		WARSTS_BATTLE,
		WARSTS_AFTERMATCH
	};
	struct ControlEvent: public BaseEvent{
		ControlEvent();
		ControlTypeEvent controlType;
	};
	#endif
}
#define LOADSAVE_FUNCTION_GENERATE() template<class Archive>  void save(Archive & ar, const unsigned int version) const; template<class Archive> void load(Archive & ar, const unsigned int version); BOOST_SERIALIZATION_SPLIT_MEMBER() 


