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
	Noise2 = new FastNoiseLite();
}

void APlanetChunk::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, Frequency) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalOctaves) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, NoiseSeed) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalLacunarity) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalGain) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, warpScale) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, Frequency2) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalOctaves2) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, NoiseSeed2) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalLacunarity2) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, FractalGain2) ||
		PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(APlanetChunk, warpScale2))
	{
		setNoiseVariables(Frequency, FractalOctaves, NoiseSeed, FractalLacunarity, FractalGain, warpScale);
		addNoise();
		Mesh->SetMaterial(0, Material);
		Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), true);
	}
}

void APlanetChunk::BeginPlay()
{
	addVertices();
	addTriangles();
	setNoiseVariables(Frequency, FractalOctaves, NoiseSeed, FractalLacunarity, FractalGain, warpScale);
	addNoise();


	Mesh->SetMaterial(0, Material);
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), true);

	Mesh->SetMaterial(1, WaterMaterial);
	Mesh->CreateMeshSection(1, MeshData.WaterVertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, MeshData.UVX, MeshData.UVY, MeshData.UVZ, MeshData.Colors, TArray<FProcMeshTangent>(), false);

}

void APlanetChunk::addVertices()
{
	MeshData.Vertices = TArray<FVector>();

	MeshData.Vertices.Add(FVector::UpVector * size);
	MeshData.WaterVertices.Add(FVector::UpVector * size * 0.99);

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
			MeshData.WaterVertices.Add(rowPoints[p].GetSafeNormal() * size * 0.99);

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
				MeshData.WaterVertices.Add(lerp.GetSafeNormal() * size * 0.99);
			}
		}
	}

	int verticeNum = MeshData.Vertices.Num();
	for (int i = 0; i < verticeNum; i++) //copy the top half to the bottom
	{
		FVector vertice = MeshData.Vertices[i];
		vertice.Z *= -1;
		MeshData.Vertices.Add(vertice);
		MeshData.WaterVertices.Add(vertice.GetSafeNormal() * size * 0.99);
	}
}

void APlanetChunk::addTriangles()
{
	MeshData.Triangles = TArray<int>();

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
	for (int i = 0; i < MeshData.Vertices.Num(); i++)
	{

		FVector ref = MeshData.Vertices[i].GetSafeNormal() * size;
		float noise = Noise->GetNoise(ref.X, ref.Y, ref.Z);
		noise = (noise + 1) / 10 + 0.9; //0.9 <= noise <= 1.1


		if (noise > 1.08)
		{
			MeshData.Colors.Add(FColor::Red);
		}
		else if (noise > 1.06)
		{
			MeshData.Colors.Add(FColor::Blue);
		}
		else if (noise > 1.04)
		{
			MeshData.Colors.Add(FColor::Emerald);
		}
		else if (noise > 1.02)
		{
			MeshData.Colors.Add(FColor::Magenta);
		}
		else if (noise > 1.01)
		{
			MeshData.Colors.Add(FColor::Cyan);
		}
		else
		{
			MeshData.Colors.Add(FColor::Black);
		}

		//add mountains / valleys
		//add rivers
		//add lakes
		MeshData.Vertices[i] = MeshData.Vertices[i].GetSafeNormal() * (size * noise);

	}
}

void APlanetChunk::setNoiseVariables(float Freq, int Octaves, int Seed, float Lac, float Gain, float warp)
{
	Noise->SetSeed(Seed);
	Noise->SetFrequency(Freq);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	Noise->SetDomainWarpAmp(warp);
	Noise->SetFractalOctaves(Octaves);
	Noise->SetFractalLacunarity(Lac);
	Noise->SetFractalGain(Gain);

	setNoiseVariables2(Frequency2, FractalOctaves2, NoiseSeed2, FractalLacunarity2, FractalGain2, warpScale2);
}

void APlanetChunk::setNoiseVariables2(float Freq, int Octaves, int Seed, float Lac, float Gain, float warp)
{
	Noise2->SetSeed(Seed);
	Noise2->SetFrequency(Freq);
	Noise2->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise2->SetFractalType(FastNoiseLite::FractalType_FBm);
	Noise2->SetDomainWarpType(FastNoiseLite::DomainWarpType_BasicGrid);
	Noise2->SetDomainWarpAmp(warp);
	Noise2->SetFractalOctaves(Octaves);
	Noise2->SetFractalLacunarity(Lac);
	Noise2->SetFractalGain(Gain);
}

