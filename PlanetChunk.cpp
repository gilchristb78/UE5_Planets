// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetChunk.h"


// Sets default values
APlanetChunk::APlanetChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	Mesh->SetCastShadow(true);
	RootComponent = Mesh;

	Noise = new FastNoiseLite();
	NoiseMountainBiome = new FastNoiseLite();
	NoiseMountain = new FastNoiseLite();
	NoisePlateau = new FastNoiseLite();
	NoisePlateauBiome = new FastNoiseLite();
	NoiseHill = new FastNoiseLite();
	NoiseHillBiome = new FastNoiseLite();

	
}

void APlanetChunk::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, Frequency) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalOctaves) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalLacunarity) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalGain) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, warpScale) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, planetSeed))
	{
		setNoiseVariables();
		addNoise();
		Mesh->SetMaterial(0, Material);
		Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), true);
	}
}

void APlanetChunk::BeginPlay()
{
	addVertices();
	addTriangles();
	setNoiseVariables();
	addNoise();

	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), true);

	Mesh->SetMaterial(1, WaterMaterial);
	Mesh->CreateMeshSection(1, MeshData.WaterVertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), false);

}

void APlanetChunk::addVertices()
{
	MeshData.Vertices.Empty();
	MeshData.WaterVertices.Empty();
	/*MeshData.Vertices = TArray<FVector>();
	MeshData.WaterVertices = TArray<FVector>();*/

	MeshData.Vertices.Add(FVector::UpVector * size);
	MeshData.WaterVertices.Add(FVector::UpVector * size);

	int resolution = 1 << subdivisions;//1 << subdivisions;


	for (int i = 1; i <= resolution; i++) //for each row going down
	{
		TArray<FVector> rowPoints = {
			FMath::LerpStable(FVector::UpVector, FVector::ForwardVector, float(i) / resolution),
			FMath::LerpStable(FVector::UpVector, FVector::RightVector, float(i) / resolution),
			FMath::LerpStable(FVector::UpVector, FVector::BackwardVector, float(i) / resolution),
			FMath::LerpStable(FVector::UpVector, FVector::LeftVector, float(i) / resolution)
		};
		
		for (int p = 0; p < 4; p++)
		{
			MeshData.Vertices.Add(rowPoints[p].GetSafeNormal() * size); //add the point
			MeshData.WaterVertices.Add(rowPoints[p].GetSafeNormal() * size);

			for (int b = 1; b < i; b++) //between points clockwise
			{
				FVector lerp;
				if (p < 3)
				{ 
					lerp = FMath::LerpStable(rowPoints[p], rowPoints[p + 1], float(b) / float(i));
				} 
				else //last one needs to use the first
					lerp = FMath::LerpStable(rowPoints[p], rowPoints[0], float(b) / float(i));

				MeshData.Vertices.Add(lerp.GetSafeNormal() * size);
				MeshData.WaterVertices.Add(lerp.GetSafeNormal() * size);
			}
		}
	}
	
	int verticeNum = MeshData.Vertices.Num();
	for (int i = 0; i < verticeNum; i++) //copy the top half to the bottom
	{
		FVector vertice = MeshData.Vertices[i];
		vertice.Z *= -1;
		MeshData.Vertices.Add(vertice);
		MeshData.WaterVertices.Add(vertice.GetSafeNormal() * size);
	}
}

void APlanetChunk::addTriangles()
{
	MeshData.Triangles.Empty();
	//MeshData.Triangles = TArray<int>();

	TArray<int> firstRow = { //first rows a bit messy
		2,1,0,3,2,0,4,3,0,1,4,0
	};

	MeshData.Triangles.Append(firstRow);


	int tr = 1;
	int br = 5;

	int resolution = 1 << subdivisions;//1 << subdivisions;

	for (int toprow = 1; toprow < resolution; toprow++) //for each row of triangles
	{
		for (int i = br; i < br + (4 * (toprow + 1)); i++) //going across bottom row 
		{

			if (i == br + (4 * (toprow + 1)) - 1) //at the end use the first one again
			{
				MeshData.Triangles.Add(br);
			}
			else
			{
				MeshData.Triangles.Add(i + 1);
			}

			MeshData.Triangles.Add(i);

			MeshData.Triangles.Add(tr);

				

			if ((i - br + 1) % (toprow + 1) != 0) //when we get to the edge of a quarter dont advance
			{
				if (tr + 1 == br)
				{
					MeshData.Triangles.Add(i + 1);
					MeshData.Triangles.Add(tr);
					MeshData.Triangles.Add(tr + 1 - (4 * toprow));
				}
				else
				{
					MeshData.Triangles.Add(i + 1);
					MeshData.Triangles.Add(tr);
					MeshData.Triangles.Add(tr + 1);
				}
				

				tr++;
				//TODO: upsidedown probably here, sometimes
			}
			if (tr == br) //at the end go back to beggining for last triangle
			{

				tr -= 4 * toprow;
			}

		}


		tr = br;
		br += 4 * (toprow + 1);

	}

	int verticeNum = MeshData.Vertices.Num() / 2; // copy the top half to the bottom
	int triangleNum = MeshData.Triangles.Num();
	for (int i = 0; i < triangleNum; i+=3)
	{
		int triangle = MeshData.Triangles[i] + verticeNum;
		int triangle2 = MeshData.Triangles[i + 1] + verticeNum;
		int triangle3 = MeshData.Triangles[i + 2] + verticeNum;
		MeshData.Triangles.Add(triangle3);
		MeshData.Triangles.Add(triangle2);
		MeshData.Triangles.Add(triangle);
	}
}

void APlanetChunk::addNoise()
{
	
	FRandomStream stream;
	stream.Initialize( 4);

	//plateaus.Add(FVector(stream.FRandRange(0, 1), stream.FRandRange(0, 1), stream.FRandRange(0, 1)).GetSafeNormal());

	float tnoise = 0;
	FVector ttempRand = FVector::UpVector;
	while (tnoise < 0.001)
	{
		ttempRand = FVector(stream.FRandRange(0, 1), stream.FRandRange(0, 1), stream.FRandRange(0, 1)).GetSafeNormal() * 1000;
		FVector tWarpedLoc = ttempRand;
		Noise->DomainWarp(tWarpedLoc.X, tWarpedLoc.Y, tWarpedLoc.Z);
		tnoise = Noise->GetNoise(tWarpedLoc.X, tWarpedLoc.Y, tWarpedLoc.Z);
		tnoise = (tnoise + 1) / 50 + 0.98;
		tnoise = FMath::Pow(FMath::Max((tnoise - 1.001) * 50, 0), 1.2);
	}

	plateaus.Add(ttempRand.GetSafeNormal());
	

	//plateaus.Add(FMath::VRand().GetSafeNormal());
	//plateaus.Add(FVector::UpVector.GetSafeNormal());

	MeshData.Colors.Empty();
	for (int i = 0; i < MeshData.Vertices.Num(); i++)
	{

		FVector ref = MeshData.Vertices[i].GetSafeNormal() * 1000;
		
		FVector WarpedLoc = ref;
		Noise->DomainWarp(WarpedLoc.X, WarpedLoc.Y, WarpedLoc.Z);
		float noise = Noise->GetNoise(WarpedLoc.X, WarpedLoc.Y, WarpedLoc.Z);
		noise = (noise + 1) / 50 + 0.98 /* 0.98 <= noise <= 1.02*/;

		FVector WarpedLocMountain = ref;
		NoiseMountain->DomainWarp(WarpedLocMountain.X, WarpedLocMountain.Y, WarpedLocMountain.Z);
		float mountainNoise = NoiseMountain->GetNoise(WarpedLocMountain.X, WarpedLocMountain.Y, WarpedLocMountain.Z);
		mountainNoise = 1 / (1 + (80 * exp(-10 * mountainNoise))); // shape the mountains to be sharp with highs and lows
		float inlandMask = FMath::Pow(FMath::Max((noise - 1.001) * 50,0),1.2);
		mountainNoise *= inlandMask;

		FVector WarpedLocMask = ref;
		NoiseMountainBiome->DomainWarp(WarpedLocMask.X, WarpedLocMask.Y, WarpedLocMask.Z);
		float noiseMask = NoiseMountainBiome->GetNoise(WarpedLocMask.X, WarpedLocMask.Y, WarpedLocMask.Z);
		noiseMask = 1 / (1 + (exp(-30 * ((noiseMask + 1) / 2 - 0.1) + 17)));
		mountainNoise *= noiseMask;
		//MeshData.Colors.Add(getColor(noiseMask * 100));

		//float plateuaNoise = FVector::Dist(plateaus[0] * 1000, ref) < 50;
		//change to 50 + noise((ref - plat[0]*1000).getsafenormal) * some number for min max width

		FVector WarpedLocPlatMask = ref;
		NoisePlateauBiome->DomainWarp(WarpedLocPlatMask.X, WarpedLocPlatMask.Y, WarpedLocPlatMask.Z);
		float noisePlatMask = NoisePlateauBiome->GetNoise(WarpedLocPlatMask.X, WarpedLocPlatMask.Y, WarpedLocPlatMask.Z);
		inlandMask = FMath::Pow(FMath::Max((noise - 0.995) * 40, 0), 0.4);
		noisePlatMask *= inlandMask;
		noisePlatMask = 1 / (1 + (exp(-50 * (noisePlatMask - 0.3))));
		float plateuaNoiseBiome = noisePlatMask * (1 - FMath::Pow(noiseMask,1)); //might need to be * noisemask < 0.1
		//add the actual smaller noiser that will be the actual plateaus (smaller than mountain range)
		//similar huge step 0 to 1, multiply by the plateua mask 

		FVector WarpedLocPlat = ref;
		NoisePlateau->DomainWarp(WarpedLocPlat.X, WarpedLocPlat.Y, WarpedLocPlat.Z);
		float noisePlat = NoisePlateau->GetNoise(WarpedLocPlat.X, WarpedLocPlat.Y, WarpedLocPlat.Z);
		float noisePlatP1 = 1 / (1 + (exp(-200 * (noisePlat - 0.1))));
		float noisePlatP2 = 1 / (1 + (exp(-200 * (noisePlat - 0.4))));
		noisePlat = noisePlatP1 + (noisePlatP2 * 0.8);

		noisePlat *= plateuaNoiseBiome;
		
		FVector WarpedLocHill = ref;
		NoiseHill->DomainWarp(WarpedLocPlat.X, WarpedLocPlat.Y, WarpedLocPlat.Z);
		float noiseHill = NoiseHill->GetNoise(WarpedLocPlat.X, WarpedLocPlat.Y, WarpedLocPlat.Z);
		noiseHill < 0 ? noiseHill = 0 : noiseHill;

		FVector WarpedLocHillMask = ref;
		NoiseHillBiome->DomainWarp(WarpedLocHillMask.X, WarpedLocHillMask.Y, WarpedLocHillMask.Z);
		float noiseHillMask = NoiseHillBiome->GetNoise(WarpedLocHillMask.X, WarpedLocHillMask.Y, WarpedLocHillMask.Z);
		inlandMask = FMath::Max((noise - 0.995) * 40, 0);
		inlandMask = 1 / (1 + (exp(-80 * (inlandMask - 0.25))));
		noiseHillMask *= inlandMask;
		noiseHillMask = 1 / (1 + (exp(-50 * (noiseHillMask - 0.2))));

		noiseHill *= noiseHillMask;
	
	
		
		
		

		/*MeshData.Colors.Add(getColor((noiseVolcano - 0.3) * 100));*/

		if (mountainNoise > 0.1)
		{
			MeshData.Colors.Add(FColor::White);
		}
		else if (mountainNoise > 0.04)
		{
			MeshData.Colors.Add(FColor(40, 40, 40));
		}
		else if (mountainNoise > 0.02f)
		{
			MeshData.Colors.Add(FColor(100, 50, 0));
		}
		else if (plateuaNoiseBiome > 0.1 && noisePlatP1 > 0.9995f && (noisePlatP2 < 0.1 || (noisePlatP2 > 0.9995f)))
		{
			MeshData.Colors.Add(FColor(100, 100, 100));
		}
		else if (plateuaNoiseBiome > 0.1 && noisePlatP1 > 0.1)
		{
			MeshData.Colors.Add(FColor(200, 175, 100));
		}
		else if (mountainNoise > 0.005f)
		{
			MeshData.Colors.Add(FColor(0, 50, 0));
		}
		else if (plateuaNoiseBiome > 0.01f && noise > 1.001)
		{
			MeshData.Colors.Add(FColor(0, 100, 0));
		}
		else if (noiseHill > 0.001)
		{
			MeshData.Colors.Add(FColor(100, 150, 100));
		}
		else if (noise > 1.001)
		{
			MeshData.Colors.Add(FColor(50,100,50));
		}
		else
		{
			MeshData.Colors.Add(FColor(250, 225, 200));
		}


		//add mountains / valleys
		//add rivers
		//add lakes
		float height = ((size * noise) + (mountainNoise * (size / 10))) + (noisePlat * (size / 120)) + (noiseHill * (size / 100));
		MeshData.Vertices[i] = MeshData.Vertices[i].GetSafeNormal() * height;

	}
}

void APlanetChunk::setNoiseVariables()
{
	/*NoiseVolcano->SetSeed(planetSeed + 7);
	NoiseVolcano->SetFrequency(Frequency);
	NoiseVolcano->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoiseVolcano->SetFractalType(FastNoiseLite::FractalType_FBm);
	NoiseVolcano->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	NoiseVolcano->SetDomainWarpAmp(warpScale);
	NoiseVolcano->SetFractalOctaves(FractalOctaves);
	NoiseVolcano->SetFractalLacunarity(FractalLacunarity);
	NoiseVolcano->SetFractalGain(FractalGain);*/
	setNoiseConstants();
}

void APlanetChunk::setNoiseConstants()
{
	Noise->SetSeed(planetSeed);
	Noise->SetFrequency(0.002f);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	Noise->SetDomainWarpAmp(450.0f);
	Noise->SetFractalOctaves(4.0f);
	Noise->SetFractalLacunarity(2.1f);
	Noise->SetFractalGain(0.5f);

	NoiseMountainBiome->SetSeed(planetSeed + 1);
	NoiseMountainBiome->SetFrequency(0.005f);
	NoiseMountainBiome->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoiseMountainBiome->SetFractalType(FastNoiseLite::FractalType_FBm);
	NoiseMountainBiome->SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2Reduced);
	NoiseMountainBiome->SetDomainWarpAmp(1.0f);
	NoiseMountainBiome->SetFractalOctaves(1.0f);
	NoiseMountainBiome->SetFractalLacunarity(2.0f);
	NoiseMountainBiome->SetFractalGain(0.5f);

	NoiseMountain->SetSeed(planetSeed + 2);
	NoiseMountain->SetFrequency(0.03f);
	NoiseMountain->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoiseMountain->SetFractalType(FastNoiseLite::FractalType_Ridged);
	NoiseMountain->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	NoiseMountain->SetDomainWarpAmp(30.0f);
	NoiseMountain->SetFractalOctaves(4.0f);
	NoiseMountain->SetFractalLacunarity(2.5f);
	NoiseMountain->SetFractalGain(0.5f);

	NoisePlateauBiome->SetSeed(planetSeed + 3);
	NoisePlateauBiome->SetFrequency(0.005f);
	NoisePlateauBiome->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoisePlateauBiome->SetFractalType(FastNoiseLite::FractalType_FBm);
	NoisePlateauBiome->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	NoisePlateauBiome->SetDomainWarpAmp(120.0f);
	NoisePlateauBiome->SetFractalOctaves(1.0f);
	NoisePlateauBiome->SetFractalLacunarity(2.0f);
	NoisePlateauBiome->SetFractalGain(0.5f);

	NoisePlateau->SetSeed(planetSeed + 4);
	NoisePlateau->SetFrequency(0.02f);
	NoisePlateau->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoisePlateau->SetFractalType(FastNoiseLite::FractalType_FBm);
	NoisePlateau->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	NoisePlateau->SetDomainWarpAmp(20.0f);
	NoisePlateau->SetFractalOctaves(1.0f);
	NoisePlateau->SetFractalLacunarity(2.0f);
	NoisePlateau->SetFractalGain(0.5f);

	NoiseHill->SetSeed(planetSeed + 5);
	NoiseHill->SetFrequency(0.05f);
	NoiseHill->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoiseHill->SetFractalType(FastNoiseLite::FractalType_FBm);
	NoiseHill->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	NoiseHill->SetDomainWarpAmp(0.0f);
	NoiseHill->SetFractalOctaves(4.0f);
	NoiseHill->SetFractalLacunarity(2.5f);
	NoiseHill->SetFractalGain(0.5f);

	NoiseHillBiome->SetSeed(planetSeed + 6);
	NoiseHillBiome->SetFrequency(0.003);
	NoiseHillBiome->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NoiseHillBiome->SetFractalType(FastNoiseLite::FractalType_FBm);
	NoiseHillBiome->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	NoiseHillBiome->SetDomainWarpAmp(0.0f);
	NoiseHillBiome->SetFractalOctaves(1.0f);
	NoiseHillBiome->SetFractalLacunarity(2.0f);
	NoiseHillBiome->SetFractalGain(0.5f);
}

FColor APlanetChunk::getColor(float percentage)
{
	
	if (percentage > 80.0f)
	{
		return FColor(255, 0, 0);
	}
	else if (percentage > 70.0f)
	{
		return FColor(255, 160, 0);
	}
	else if (percentage > 60.0f)
	{
		return FColor(255, 255, 0);
	}
	else if (percentage > 50.0f)
	{
		return FColor(0, 255, 0);
	}
	else if (percentage > 40.0f)
	{
		return FColor(0, 255, 255);
	}
	else if (percentage > 30.0f)
	{
		return FColor(0, 0, 255);
	}
	else if (percentage > 20.0f)
	{
		return FColor(100, 0, 255);
	}
	else if (percentage > 10.0f)
	{
		return FColor(255, 0, 255);
	}
	else if (percentage > 1.0f)
	{
		return FColor(255, 180, 255);
	}
	else
	{
		return FColor(100, 100, 100);
	}
	

}

