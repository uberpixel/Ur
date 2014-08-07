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
		RN::Model *model = RN::Model::WithFile("Models/Ship/ship_radar.sgm");
		model->GetMaterialAtIndex(0, 0)->SetBlending(true);
		model->GetMaterialAtIndex(0, 0)->SetBlendMode(RN::Material::BlendMode::One, RN::Material::BlendMode::One);
		model->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
		model->GetMaterialAtIndex(0, 0)->SetLighting(false);
		model->GetMaterialAtIndex(0, 0)->SetDepthWrite(false);
		SetModel(model);
		SetScale(RN::Vector3(0.15f));
		SetRenderGroup(2);
		
		
		_shipModel = new RN::Model(RN::Model::WithFile("Models/Ship/ship_outside.sgm"));
		_shipModel->GetMaterialAtIndex(0, 0)->SetLighting(false);
		_shipModel->GetMaterialAtIndex(0, 0)->SetBlending(true);
		_shipModel->GetMaterialAtIndex(0, 0)->SetBlendMode(RN::Material::BlendMode::One, RN::Material::BlendMode::One);
		_shipModel->GetMaterialAtIndex(0, 0)->SetDiffuseColor(RN::Color::Red());
		_shipModel->GetMaterialAtIndex(0, 0)->SetDepthWrite(false);
		_shipModel->GetMaterialAtIndex(0, 1)->SetLighting(false);
		_shipModel->GetMaterialAtIndex(0, 1)->SetBlending(true);
		_shipModel->GetMaterialAtIndex(0, 1)->SetBlendMode(RN::Material::BlendMode::One, RN::Material::BlendMode::One);
		_shipModel->GetMaterialAtIndex(0, 1)->SetDiffuseColor(RN::Color::Red());
		_shipModel->GetMaterialAtIndex(0, 1)->SetDepthWrite(false);
		
		RN::Model *shipModelWhite = new RN::Model(_shipModel);
		shipModelWhite->Autorelease();
		shipModelWhite->GetMaterialAtIndex(0, 0)->SetDiffuseColor(RN::Color::Gray());
		shipModelWhite->GetMaterialAtIndex(0, 1)->SetDiffuseColor(RN::Color::Gray());
		_ships.push_back(new RN::Entity(shipModelWhite));
		_ships.back()->SetScale(RN::Vector3(0.002f));
		_ships.back()->SetRenderGroup(2);
	}
	
	Radar::~Radar()
	{
		_shipModel->Release();
		
		while(_ships.size() > 0)
		{
			_ships.back()->Release();
			_ships.pop_back();
		}
	}
	
	void Radar::Update(float delta)
	{
		if(_ships.size() <= World::GetSharedInstance()->GetEnemies()->GetCount())
		{
			_ships.push_back(new RN::Entity(_shipModel));
			_ships.back()->SetScale(RN::Vector3(0.002f));
			_ships.back()->SetRenderGroup(2);
		}
		
		while(_ships.size()-1 > World::GetSharedInstance()->GetEnemies()->GetCount())
		{
			_ships.back()->Release();
			_ships.pop_back();
		}
	}
	
	void Radar::DidUpdate(ChangeSet changeSet)
	{
		RN::Entity::DidUpdate(changeSet);
		
		if(_ships.size() == 0)
			return;
		
		_ships.front()->SetPosition(GetWorldPosition());
		_ships.front()->SetRotation(SpaceShip::GetLocalShip()->GetWorldRotation());
		
		if(changeSet == ChangeSet::Position)
		{
			RN::Array *enemies = World::GetSharedInstance()->GetEnemies();
			
			enemies->Enumerate<Enemy>([&](Enemy *enemy, size_t index, bool &stop) {
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
