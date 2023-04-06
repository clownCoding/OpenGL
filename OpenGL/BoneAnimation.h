#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <assert.h>

class BoneAnimation {
private:
	double startTime;
	Assimp::Importer import;
	const aiScene* scene;

	unsigned int FindScaling(double AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindRotation(double AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int FindPosition(double AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, double AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, double AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, double AnimationTime, const aiNodeAnim* pNodeAnim);
	void LoadAnimation(std::string filePath);
	
public:
	inline BoneAnimation(std::string filePath):startTime(0) { LoadAnimation(filePath); }
	void SetStartTime();
	aiMatrix4x4 GetAimationTransform(double timeInTicks, const aiNodeAnim* nodeAnim);
	const aiNodeAnim* FindNodeAnim(const std::string& NodeName);
	bool HasAnimation();
	double GetAnimationTimeTicks();

};
