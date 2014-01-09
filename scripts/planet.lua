planet= nil
coreManager= nil
assetMng= nil
unitMng = nil
function setplanet(planetNew)
	planet =planetNew

end
function setcoreman(coreNew)
	coreManager =coreNew

end
function setAsset(assetNew)
	assetMng= assetNew
end 
function setUnitMan(unitMngNew)
	unitMng= unitMngNew
end
function test()
	res =true
	for v in planet.allCountry do 
		for k in v.allDeposit do
			res = res and(k.open)
		end

	end
	return res
end

dayAnotherProduction = {}

function buildingPass(buildin) 
	if buildin.btype==7 then
		if buildin:hasUpdate("depofart") then
			if dayAnotherProduction[buildin.name]==nil then dayAnotherProduction[buildin.name]=1 end
			
			if dayAnotherProduction[buildin.name]%3==0 then
				coreManager:randomresource(1)
			end
			if dayAnotherProduction[buildin.name]%7==0 then
				building.core.country:changestate("IIF",1)
			end
						
		end
			
	end
end