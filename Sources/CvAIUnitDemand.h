#pragma once

#ifndef CV_AI_UNIT_DEMAND_H
#define CV_AI_UNIT_DEMAND_H

#include "CvEnums.h"
#include "CvUnitSelectionCriteria.h"

// Temporary validation guard.  Undefine to route migrated city call sites
// through their legacy Contract Broker advertisement path.
#define USE_AI_UNIT_DEMAND_ACCOUNTING
#define USE_AI_UNIT_DEMAND_WAVE2

enum AIUnitDemandScopeTypes
{
	AI_UNIT_DEMAND_PLAYER,
	AI_UNIT_DEMAND_LAND_AREA,
	AI_UNIT_DEMAND_WATER_AREA
};

enum AIUnitDemandSelectorTypes
{
	AI_UNIT_DEMAND_BY_ROLE,
	AI_UNIT_DEMAND_BY_EXACT_UNIT
};

enum AIUnitDemandMeasureTypes
{
	AI_UNIT_DEMAND_MEASURE_OBJECT_COUNT,
	AI_UNIT_DEMAND_MEASURE_FORMATION_VOLUME,
	AI_UNIT_DEMAND_MEASURE_CARGO_CAPACITY,
	AI_UNIT_DEMAND_MEASURE_CARGO_VOLUME,
	NUM_AI_UNIT_DEMAND_MEASURES
};

struct AIUnitDemandSupplyIndex
{
	AIUnitDemandSupplyIndex()
		: eUnitAI(NO_UNITAI)
		, eMeasure(AI_UNIT_DEMAND_MEASURE_OBJECT_COUNT)
		, eScope(AI_UNIT_DEMAND_PLAYER)
		, iScopeId(-1)
	{}

	AIUnitDemandSupplyIndex(UnitAITypes eUnitAI_, AIUnitDemandMeasureTypes eMeasure_, AIUnitDemandScopeTypes eScope_, int iScopeId_)
		: eUnitAI(eUnitAI_)
		, eMeasure(eMeasure_)
		, eScope(eScope_)
		, iScopeId(iScopeId_)
	{}

	bool operator<(const AIUnitDemandSupplyIndex& kOther) const
	{
		if (eUnitAI != kOther.eUnitAI) return eUnitAI < kOther.eUnitAI;
		if (eMeasure != kOther.eMeasure) return eMeasure < kOther.eMeasure;
		if (eScope != kOther.eScope) return eScope < kOther.eScope;
		return iScopeId < kOther.iScopeId;
	}

	bool operator==(const AIUnitDemandSupplyIndex& kOther) const
	{
		return eUnitAI == kOther.eUnitAI
			&& eMeasure == kOther.eMeasure
			&& eScope == kOther.eScope
			&& iScopeId == kOther.iScopeId;
	}

	UnitAITypes eUnitAI;
	AIUnitDemandMeasureTypes eMeasure;
	AIUnitDemandScopeTypes eScope;
	int iScopeId;
};

enum AIUnitDemandClassTypes
{
	AI_UNIT_DEMAND_EMERGENCY,
	AI_UNIT_DEMAND_ESSENTIAL,
	AI_UNIT_DEMAND_STRATEGIC,
	AI_UNIT_DEMAND_OPTIONAL,
	NUM_AI_UNIT_DEMAND_CLASSES
};

enum AIUnitDemandPolicyTypes
{
	AI_UNIT_DEMAND_POLICY_SETTLER,
	AI_UNIT_DEMAND_POLICY_WORKER,
	AI_UNIT_DEMAND_POLICY_SEA_WORKER,
	AI_UNIT_DEMAND_POLICY_HUNTER,
	AI_UNIT_DEMAND_POLICY_EXPLORE,
	AI_UNIT_DEMAND_POLICY_EXPLORE_SEA,
	AI_UNIT_DEMAND_POLICY_SPY,
	AI_UNIT_DEMAND_POLICY_INFILTRATOR,
	AI_UNIT_DEMAND_POLICY_SPREAD_UNIT,
	AI_UNIT_DEMAND_POLICY_ATTACK,
	AI_UNIT_DEMAND_POLICY_ATTACK_CITY,
	AI_UNIT_DEMAND_POLICY_COLLATERAL,
	AI_UNIT_DEMAND_POLICY_NAVAL_TRANSPORT,
	AI_UNIT_DEMAND_POLICY_NAVAL_ESCORT,
	AI_UNIT_DEMAND_POLICY_NAVAL_ATTACK,
	AI_UNIT_DEMAND_POLICY_CARRIER,
	AI_UNIT_DEMAND_POLICY_MISSILE_CARRIER,
	AI_UNIT_DEMAND_POLICY_CARRIER_AIR,
	AI_UNIT_DEMAND_POLICY_MISSILE_AIR,
	AI_UNIT_DEMAND_POLICY_ATTACK_AIR,
	AI_UNIT_DEMAND_POLICY_NUCLEAR,
	AI_UNIT_DEMAND_POLICY_PIRATE,
	AI_UNIT_DEMAND_POLICY_SETTLER_SEA,
	AI_UNIT_DEMAND_POLICY_SPY_SEA,
	AI_UNIT_DEMAND_POLICY_MISSIONARY_SEA
};

enum AIUnitDemandAdmissionReasonTypes
{
	AI_UNIT_DEMAND_ADMISSION_ACCEPTED,
	AI_UNIT_DEMAND_ADMISSION_REFRESHED,
	AI_UNIT_DEMAND_ADMISSION_NO_DEFICIT,
	AI_UNIT_DEMAND_ADMISSION_PARTIALLY_GATED,
	AI_UNIT_DEMAND_ADMISSION_ECONOMIC_GATE,
	AI_UNIT_DEMAND_ADMISSION_INVALID_SCOPE,
	AI_UNIT_DEMAND_ADMISSION_INVALID_SELECTOR,
	AI_UNIT_DEMAND_ADMISSION_INVALID_TARGET,
	AI_UNIT_DEMAND_ADMISSION_INCOMPATIBLE_POLICY
};

enum AIUnitDemandFulfillmentReasonTypes
{
	AI_UNIT_DEMAND_FULFILLMENT_COMMITTED,
	AI_UNIT_DEMAND_FULFILLMENT_EXPIRED,
	AI_UNIT_DEMAND_FULFILLMENT_INVALIDATED,
	AI_UNIT_DEMAND_FULFILLMENT_NO_TENDER,
	AI_UNIT_DEMAND_FULFILLMENT_NO_BUILDABLE_UNIT,
	AI_UNIT_DEMAND_FULFILLMENT_NO_PATH,
	AI_UNIT_DEMAND_FULFILLMENT_ECONOMIC_GATE,
	AI_UNIT_DEMAND_FULFILLMENT_QUEUE_REJECTED
};

enum AIUnitDemandEconomyStateTypes
{
	AI_UNIT_DEMAND_ECONOMY_NORMAL,
	AI_UNIT_DEMAND_ECONOMY_SATURATED,
	AI_UNIT_DEMAND_ECONOMY_FINANCIAL_TROUBLE,
	AI_UNIT_DEMAND_ECONOMY_CRITICAL_GOLD,
	AI_UNIT_DEMAND_ECONOMY_STRIKE
};

struct AIUnitDemandTarget
{
	AIUnitDemandTarget()
	{
		for (int iI = 0; iI < NUM_AI_UNIT_DEMAND_CLASSES; ++iI)
		{
			aiDesiredThroughClass[iI] = 0;
		}
	}

	void setDesiredThrough(AIUnitDemandClassTypes eClass, int iDesired)
	{
		for (int iI = (int)eClass; iI < NUM_AI_UNIT_DEMAND_CLASSES; ++iI)
		{
			aiDesiredThroughClass[iI] = std::max(aiDesiredThroughClass[iI], iDesired);
		}
	}

	bool isValid() const
	{
		int iPrevious = 0;
		for (int iI = 0; iI < NUM_AI_UNIT_DEMAND_CLASSES; ++iI)
		{
			if (aiDesiredThroughClass[iI] < iPrevious)
			{
				return false;
			}
			iPrevious = aiDesiredThroughClass[iI];
		}
		return true;
	}

	int aiDesiredThroughClass[NUM_AI_UNIT_DEMAND_CLASSES];
};

struct AIUnitDemandKey
{
	AIUnitDemandKey()
		: ePolicy(AI_UNIT_DEMAND_POLICY_SETTLER)
		, eSelector(AI_UNIT_DEMAND_BY_ROLE)
		, eUnitAI(NO_UNITAI)
		, eUnit(NO_UNIT)
		, eMeasure(AI_UNIT_DEMAND_MEASURE_OBJECT_COUNT)
		, eScope(AI_UNIT_DEMAND_PLAYER)
		, iScopeId(-1)
	{}

	bool operator==(const AIUnitDemandKey& kOther) const
	{
		return ePolicy == kOther.ePolicy
			&& eSelector == kOther.eSelector
			&& eUnitAI == kOther.eUnitAI
			&& eUnit == kOther.eUnit
			&& eMeasure == kOther.eMeasure
			&& eScope == kOther.eScope
			&& iScopeId == kOther.iScopeId
			&& criteria == kOther.criteria;
	}

	bool operator<(const AIUnitDemandKey& kOther) const
	{
		if (ePolicy != kOther.ePolicy) return ePolicy < kOther.ePolicy;
		if (eSelector != kOther.eSelector) return eSelector < kOther.eSelector;
		if (eUnitAI != kOther.eUnitAI) return eUnitAI < kOther.eUnitAI;
		if (eUnit != kOther.eUnit) return eUnit < kOther.eUnit;
		if (eMeasure != kOther.eMeasure) return eMeasure < kOther.eMeasure;
		if (eScope != kOther.eScope) return eScope < kOther.eScope;
		if (iScopeId != kOther.iScopeId) return iScopeId < kOther.iScopeId;
		return criteria < kOther.criteria;
	}

	AIUnitDemandPolicyTypes ePolicy;
	AIUnitDemandSelectorTypes eSelector;
	UnitAITypes eUnitAI;
	UnitTypes eUnit;
	AIUnitDemandMeasureTypes eMeasure;
	AIUnitDemandScopeTypes eScope;
	int iScopeId;
	CvUnitSelectionCriteria criteria;
};

struct AIUnitRoleSupply
{
	AIUnitRoleSupply()
		: iExisting(0)
		, iTraining(0)
		, iOutstandingProduction(0)
		, iEffectiveSupply(0)
	{}

	int iExisting;
	int iTraining;
	int iOutstandingProduction;
	int iEffectiveSupply;
};

struct AIUnitDemandResult
{
	AIUnitDemandResult()
		: iRequestId(-1)
		, iAcceptedQuantity(0)
		, iOutstandingQuantity(0)
		, iEconomicallyGatedQuantity(0)
		, iRemainingDeficit(0)
		, eReason(AI_UNIT_DEMAND_ADMISSION_INVALID_TARGET)
	{}

	int iRequestId;
	int iAcceptedQuantity;
	int iOutstandingQuantity;
	int iEconomicallyGatedQuantity;
	int iRemainingDeficit;
	AIUnitRoleSupply kSupply;
	AIUnitDemandAdmissionReasonTypes eReason;
};

#endif
