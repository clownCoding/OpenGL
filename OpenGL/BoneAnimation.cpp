#include "BoneAnimation.h"

void BoneAnimation::LoadAnimation(std::string filePath)
{
	scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
}

void BoneAnimation::SetStartTime()
{
	this->startTime = glfwGetTime();
}

const aiNodeAnim* BoneAnimation::FindNodeAnim(const std::string& NodeName)
{
	const aiAnimation* pAnimation = scene->mAnimations[0];
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* nodeAnim = pAnimation->mChannels[i];

		if (std::string(nodeAnim->mNodeName.data) == NodeName) {
			return nodeAnim;
		}
	}

	return nullptr;
}

unsigned int BoneAnimation::FindScaling(double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

unsigned int BoneAnimation::FindRotation(double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

unsigned int BoneAnimation::FindPosition(double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
		if (AnimationTime < t) {
			return i;
		}
	}

	return 0;
}

void BoneAnimation::CalcInterpolatedScaling(aiVector3D& Out, double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = ScalingIndex + 1;
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
	float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - (float)t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void BoneAnimation::CalcInterpolatedRotation(aiQuaternion& Out, double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = RotationIndex + 1;
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
	float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out.Normalize();
}

void BoneAnimation::CalcInterpolatedPosition(aiVector3D& Out, double AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = PositionIndex + 1;
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
	float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTime - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

aiMatrix4x4 BoneAnimation::GetAimationTransform(double timeInTicks, const aiNodeAnim* nodeAnim)
{
	aiMatrix4x4 transform;

	if (nodeAnim) {
		aiVector3D scaling;
		CalcInterpolatedScaling(scaling, timeInTicks, nodeAnim);
		aiMatrix4x4 scalingM;
		scalingM.Scaling(scaling, scalingM);

		aiQuaternion rotation;
		CalcInterpolatedRotation(rotation, timeInTicks, nodeAnim);
		aiMatrix4x4 rotationM = aiMatrix4x4(rotation.GetMatrix());

		aiVector3D translation;
		CalcInterpolatedPosition(translation, timeInTicks, nodeAnim);
		aiMatrix4x4 translationM;
		translationM.Translation(translation, translationM);

		transform = translationM * rotationM * scalingM;
	}
	return transform;
}

double BoneAnimation::GetAnimationTimeTicks()
{
	double currentTime = glfwGetTime();
	double animationTimeSec = currentTime - startTime;
	double ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0;
	double timeInTicks = animationTimeSec * ticksPerSecond;
	double animationTimeTicks = fmod(timeInTicks, scene->mAnimations[0]->mDuration);

	return animationTimeTicks;
}

bool BoneAnimation::HasAnimation() {
	if (scene->mAnimations != 0) {
		return true;
	}
	else {
		return false;
	}
}
