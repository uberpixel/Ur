//
//  URRadar.cpp
//  Ur
//
//  Created by Nils Daumann on 20.07.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "URRadar.h"
#include "URWorld.h"

namespace UR
{
	Radar::Radar()
	{
        AddFlags(RN::Entity::Flags::DrawLate);
        
		RN::Model *model = RN::Model::WithName(RNCSTR("Models/Ship/ship_radar.sgm"));
        RN::Model::LODStage *lodStage = model->GetLODStage(0);
        lodStage->GetMaterialAtIndex(0)->SetBlendOperation(RN::BlendOperation::Add, RN::BlendOperation::Add);
        lodStage->GetMaterialAtIndex(0)->SetBlendFactorSource(RN::BlendFactor::One, RN::BlendFactor::Zero);
        lodStage->GetMaterialAtIndex(0)->SetBlendFactorDestination(RN::BlendFactor::One, RN::BlendFactor::One);
        lodStage->GetMaterialAtIndex(0)->SetCullMode(RN::CullMode::None);
        lodStage->GetMaterialAtIndex(0)->SetDepthWriteEnabled(false);
		SetModel(model);
		SetScale(RN::Vector3(0.15f));
		
		
		_shipModel = RN::Model::WithName(RNCSTR("Models/Ship/ship_outside.sgm"));
        _shipModel = World::GetSharedInstance()->MakeDeepCopy(_shipModel)->Retain();
        lodStage = _shipModel->GetLODStage(0);
        lodStage->GetMaterialAtIndex(0)->SetBlendOperation(RN::BlendOperation::Add, RN::BlendOperation::Add);
        lodStage->GetMaterialAtIndex(0)->SetBlendFactorSource(RN::BlendFactor::One, RN::BlendFactor::Zero);
        lodStage->GetMaterialAtIndex(0)->SetBlendFactorDestination(RN::BlendFactor::One, RN::BlendFactor::One);
        lodStage->GetMaterialAtIndex(0)->SetCullMode(RN::CullMode::None);
        lodStage->GetMaterialAtIndex(0)->SetDepthWriteEnabled(false);
        lodStage->GetMaterialAtIndex(0)->SetDiffuseColor(RN::Color::Red());
        lodStage->GetMaterialAtIndex(1)->SetBlendOperation(RN::BlendOperation::Add, RN::BlendOperation::Add);
        lodStage->GetMaterialAtIndex(1)->SetBlendFactorSource(RN::BlendFactor::One, RN::BlendFactor::Zero);
        lodStage->GetMaterialAtIndex(1)->SetBlendFactorDestination(RN::BlendFactor::One, RN::BlendFactor::One);
        lodStage->GetMaterialAtIndex(1)->SetCullMode(RN::CullMode::None);
        lodStage->GetMaterialAtIndex(1)->SetDepthWriteEnabled(false);
        lodStage->GetMaterialAtIndex(1)->SetDiffuseColor(RN::Color::Red());
		
        RN::Model *shipModelWhite = World::GetSharedInstance()->MakeDeepCopy(_shipModel);
        lodStage = shipModelWhite->GetLODStage(0);
        lodStage->GetMaterialAtIndex(0)->SetDiffuseColor(RN::Color::Gray());
        lodStage->GetMaterialAtIndex(0)->SetDiffuseColor(RN::Color::Gray());
        RN::Entity *ship = new RN::Entity(shipModelWhite);
        ship->AddFlags(RN::Entity::Flags::DrawLate);
        ship->SetScale(RN::Vector3(0.005f));
		_ships.push_back(ship);
        AddChild(ship->Autorelease());
	}
	
	Radar::~Radar()
	{
		_shipModel->Release();
	}
	
	void Radar::Update(float delta)
	{
		if(_ships.size() <= World::GetSharedInstance()->GetEnemies()->GetCount())
		{
            RN::Entity *ship = new RN::Entity(_shipModel);
            ship->AddFlags(RN::Entity::Flags::DrawLate);
			_ships.push_back(ship);
            AddChild(ship->Autorelease());
			_ships.back()->SetScale(RN::Vector3(0.005f));
		}
		
		while(_ships.size()-1 > World::GetSharedInstance()->GetEnemies()->GetCount())
		{
            RN::Entity *ship = _ships.back();
			_ships.pop_back();
            RemoveChild(ship);
		}
	}
	
	void Radar::DidUpdate(ChangeSet changeSet)
	{
		RN::Entity::DidUpdate(changeSet);
		
		if(_ships.size() == 0 || !SpaceShip::GetLocalShip())
			return;
		
		_ships.front()->SetWorldRotation(SpaceShip::GetLocalShip()->GetWorldRotation());
		
		if(changeSet == ChangeSet::Position)
		{
			RN::Array *enemies = World::GetSharedInstance()->GetEnemies();
			
			enemies->Enumerate<Enemy>([&](Enemy *enemy, size_t index, bool &stop) {
                
                if(index + 1 >= _ships.size())
                {
                    stop = true;
                    return;
                }
                
				RN::Vector3 diff = enemy->GetWorldPosition()-GetWorldPosition();
				if(diff.GetLength() < 800.0f)
				{
					_ships[index+1]->SetWorldPosition(GetWorldPosition()+diff/800.0f*GetBoundingSphere().radius);
					_ships[index+1]->SetWorldRotation(enemy->GetWorldRotation());
					_ships[index+1]->SetFlags(_ships[index+1]->GetFlags()&~RN::SceneNode::Flags::Hidden);
				}
				else
				{
					_ships[index+1]->SetFlags(_ships[index+1]->GetFlags()|RN::SceneNode::Flags::Hidden);
				}
			});
		}
	}
}
