#pragma once

#ifndef CvContractBroker_h__
#define CvContractBroker_h__

#include "CvAIUnitDemand.h"
#include "CvUnitSelectionCriteria.h"

class CvCity;
class CvUnit;

//	Define this to have cities advertise units needs and tender for the business of building them
#define	USE_UNIT_TENDERING

//	Capability flags - may be ORd together
typedef enum	unitCapabilities
{
	NO_UNITCAPABILITIES = 0,
	DEFENSIVE_UNITCAPABILITIES = 1 << 0,
	OFFENSIVE_UNITCAPABILITIES = 1 << 1,
	WORKER_UNITCAPABILITIES = 1 << 2,
	HEALER_UNITCAPABILITIES = 1 << 3
} unitCapabilities;

enum ContractWorkRequestKindTypes
{
	CONTRACT_WORK_REQUEST_PERSISTENT_OPERATIONAL,
	CONTRACT_WORK_REQUEST_TRANSIENT_CITY_PRODUCTION
};

//	Structures used to hold work requests and looking-for-work unit info
typedef struct
{
	int						iPriority;
	unitCapabilities		eUnitFlags;
	UnitAITypes				eAIType;
	UnitTypes				eUnitType;
	int						iAtX;
	int						iAtY;
	int						iMaxPath;
	int						iUnitId;
	int						iWorkRequestId;
	int						iRequiredStrengthTimes100;
	bool					bFulfilled;
	ContractWorkRequestKindTypes eRequestKind;
	int						iCreatedTurn;
	int						iLastRefreshTurn;
	int						iSourceCityId;
	CvUnitSelectionCriteria	criteria;
} workRequest;

struct AIUnitDemandRoleIndex
{
	AIUnitDemandRoleIndex()
		: eUnitAI(NO_UNITAI)
		, eScope(AI_UNIT_DEMAND_PLAYER)
		, iScopeId(-1)
	{}

	AIUnitDemandRoleIndex(UnitAITypes eUnitAI_, AIUnitDemandScopeTypes eScope_, int iScopeId_)
		: eUnitAI(eUnitAI_)
		, eScope(eScope_)
		, iScopeId(iScopeId_)
	{}

	bool operator<(const AIUnitDemandRoleIndex& kOther) const
	{
		if (eUnitAI != kOther.eUnitAI) return eUnitAI < kOther.eUnitAI;
		if (eScope != kOther.eScope) return eScope < kOther.eScope;
		return iScopeId < kOther.iScopeId;
	}

	UnitAITypes eUnitAI;
	AIUnitDemandScopeTypes eScope;
	int iScopeId;
};

struct AIProductionDemand
{
	AIProductionDemand()
		: iRequestId(-1)
		, iPriority(0)
		, iMaxUnitSpendingPercent(0)
		, iSourceCityId(-1)
		, iOutstandingQuantity(0)
		, iCreatedTurn(-1)
		, iLastRefreshTurn(-1)
		, bInvalid(false)
	{}

	int iRequestId;
	AIUnitDemandKey kKey;
	AIUnitDemandTarget kTarget;
	int iPriority;
	int iMaxUnitSpendingPercent;
	int iSourceCityId;
	int iOutstandingQuantity;
	int iCreatedTurn;
	int iLastRefreshTurn;
	bool bInvalid;
};

typedef struct
{
	int					iUnitId;
	UnitTypes			eUnitType;
	int					iDefensiveValue;
	int					iOffensiveValue;
	bool				bIsWorker;
	bool				bIsHealer;
	int					iAtX;
	int					iAtY;
	int					iContractedWorkRequest;
	int					iMatchedToRequestSeqThisPlot;
	int					iMatchedToRequestSeqAnyPlot;
	int					iMinPriority;
} advertisingUnit;

typedef struct
{
	int					iCityId;
	int					iMinPriority;
} cityTender;

//	Highest priority city builds have this priority and work down from there
#define	CITY_BUILD_PRIORITY_CEILING				900
#define	MINIMUM_CAPITAL_DEFENDER_PRIORITY		800
#define HIGHEST_PRIORITY_ESCORT_PRIORITY		600
#define MINIMUM_CITY_DEFENDER_PRIORITY			500
#define HIGH_PRIORITY_ESCORT_PRIORITY			400
#define	CITY_NO_WORKERS_WORKER_PRIORITY			300
#define FLOATING_DEFENDER_REQUEST_PRIORITY_MAX	200
#define LOW_PRIORITY_ESCORT_PRIORITY			100

//
//	The CvContractBroker abstracts a communication that allows 'idle' units to advertise themselves
//	as 'looking for work', and for requests for units to be placed with priority and criteria.  This
//	allows for cooperative behavior without both parties needing to be fully aware of the logic, and
//	reduces the need for searching when a unit looks for the best activity to undertake, which helps
//	performance
//
//	Intended usage is in a 2-pass process (from CvUnitAI::AI_update()).  In the unit's activity choice AI
//	it should:
//		1)	If it has high priority work determined directly by its own unit AI push that mission
//		2)	If no high priority work advertise itself with this broker as available for work and return
//			from AI_update() with no mission pushed (which measn it'll get called again once all the units
//			have been cycled through)
//		3)	On the second call ask for the best work from this broker and if provided push a mission
//			to do what is asked of it (move to a location, join a group being current options)
//		4)	If no suitable work is available continue with its own unit AI for low priority actions
//
class CvContractBroker : bst::noncopyable
{
public:
	CvContractBroker();
	virtual ~CvContractBroker();

	//	Initialize
	void	init(PlayerTypes eOwner);

	//	Delete all work requests and looking for work records
	void	reset();
	void	cleanup();
	bool    alreadyLookingForWork(const CvUnit* pUnit);
	//	Note a unit looking for work
	void	lookingForWork(const CvUnit* pUnit, int iMinPriority = 0);

	//	Unit fulfilled its work and is no longer advertising as available
	void	removeUnit(const CvUnit* pUnit);
	void	removeUnit(const int iUnitId);

	//	Make a work request
	//		iPriority should be in the range 0-1000 ideally
	//		eUnitFlags indicate the type(s) of unit sought
	//		(iAtX,iAtY) is (roughly) where the work will be
	//		pJoinUnit may be NULL but if not it is a request to join that unit's group
	void	advertiseWork(int iPriority, unitCapabilities eUnitFlags, int iAtX, int iAtY, const CvUnit* pJoinUnit, UnitAITypes eAIType = NO_UNITAI, int iUnitStrength = -1, const CvUnitSelectionCriteria* criteria = NULL, int iMaxPath = MAX_INT, ContractWorkRequestKindTypes eRequestKind = CONTRACT_WORK_REQUEST_PERSISTENT_OPERATIONAL, int iSourceCityId = -1);
	void	beginProductionDemandCycle();
	AIUnitDemandResult upsertProductionDemand(const AIUnitDemandKey& kKey, const AIUnitDemandTarget& kTarget, int iPriority, int iMaxUnitSpendingPercent, int iSourceCityId, int iBaseSupply);
	int		getOutstandingProduction(UnitAITypes eUnitAI, AIUnitDemandScopeTypes eScope, int iScopeId) const;
	int		getProductionDemandCount() const { return (int)m_productionDemands.size(); }
	bool	validateProductionDemandIndexes() const;
	//	Advertise a tender to build units
	//		iMinPriority indicates the lowest priority request this tender is appropriate for
	void	advertiseTender(const CvCity* pCity, int iMinPriority);
	//	Find out how many requests have already been made for units of a specified AI type
	//	This is used by cities requesting globally needed units like settlers to avoid multiple
	//	tenders all occurring at once
	int		numRequestsOutstanding(UnitAITypes eUnitAI, bool bAtCityOnly = true, const CvPlot* pPlot = NULL) const;
	//	Make a contract
	//	This will attempt to make the best contracts between currently
	//	advertising units and work, then search the resulting set for the work
	//	of the requested unit
	//	returns true if a contract is made along with the details of what to do
	bool	makeContract(CvUnit* pUnit, int& iAtX, int& iAtY, CvUnit*& pJoinUnit, bool bThisPlotOnly);
	void	finalizeTenderContracts();
	void postProcessUnitsLookingForWork();

private:

	void log(int level, char* format, ...);
	void internalRemoveUnit(const int unitId);
	void changeOutstandingProduction(const AIUnitDemandKey& kKey, int iChange);
	void finalizeProductionDemands(std::vector<bool>& tenderUsed);

	const workRequest* findWorkRequest(int iWorkRequestId) const;
	workRequest* findWorkRequestByUnitId(int userId);
	advertisingUnit* findBestUnit(const workRequest& request, bool bThisPlotOnly);
	CvUnit* findUnit(int iUnitId) const;
	int					lowerPartiallyFulfilledRequestPriority(int iPreviousPriority, int iPreviousRequestStrength, int iStrengthProvided) const;
	UnitValueFlags		unitCapabilities2UnitValueFlags(unitCapabilities eCapabilities) const;

	PlayerTypes						m_eOwner;
	const wchar_t					*m_ownerName;
	std::vector<workRequest>		m_workRequests;
	std::map<AIUnitDemandKey, AIProductionDemand> m_productionDemands;
	std::map<AIUnitDemandRoleIndex, int> m_outstandingProductionByRole;
	std::vector<advertisingUnit>	m_advertisingUnits;
	std::vector<cityTender>			m_advertisingTenders;
	std::map<int, bool>				m_contractedUnits;
	int								m_iNextWorkRequestId;
	int								m_iNextProductionDemandId;
	int								m_iEmployedUnits;
};

#endif // CvContractBroker_h__
