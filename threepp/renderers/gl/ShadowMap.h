//
// Created by byter on 13.09.17.
//

#ifndef THREEPP_SHADOWMAP_H
#define THREEPP_SHADOWMAP_H

#include <threepp/core/Object3D.h>
#include <threepp/math/Frustum.h>
#include <threepp/math/Vector2.h>
#include <threepp/math/Vector4.h>
#include <threepp/material/Material.h>
#include <threepp/material/MeshDepthMaterial.h>
#include <threepp/material/MeshDistanceMaterial.h>
#include <threepp/light/Light.h>
#include <threepp/scene/Scene.h>
#include <threepp/camera/PerspectiveCamera.h>

#include "Objects.h"
#include "RenderTarget.h"
#include "Capabilities.h"

namespace three {
namespace gl {

class Renderer_impl;

class ShadowMap
{
  math::Frustum _frustum;
  math::Vector3 _lightPositionWorld;

  enum Flag : uint16_t {Morphing = 1, Skinning= 2};

  std::vector<Material::Ptr> _depthMaterials;
  std::vector<Material::Ptr> _distanceMaterials;

  std::unordered_map<sole::uuid, std::unordered_map<sole::uuid, Material::Ptr>> _materialCache;

  math::Vector3 _cubeDirections[6] {
     math::Vector3(1, 0, 0), math::Vector3(-1, 0, 0), math::Vector3(0, 0, 1),
     math::Vector3(0, 0, -1), math::Vector3(0, 1, 0), math::Vector3(0, -1, 0)
  };

  math::Vector3 _cubeUps[6] {
     math::Vector3(0, 1, 0), math::Vector3(0, 1, 0), math::Vector3(0, 1, 0),
     math::Vector3(0, 1, 0), math::Vector3(0, 0, 1), math::Vector3(0, 0, -1)
  };

  math::Vector4 _cube2DViewPorts[6];

  bool _needsRender = false;

  unsigned _faceCount;

  ShadowMapType _type = ShadowMapType::PCFSoft;

  Renderer_impl &_renderer;
  Objects &_objects;

  const Capabilities &_capabilities;

public:
  bool enabled = false;
  bool needsUpdate = true;
  bool autoUpdate = true;

  ShadowMap(Renderer_impl &renderer, Objects &objects, Capabilities &capabilities);

  void setup(std::vector<Light::Ptr> lights, Scene::Ptr scene, Camera::Ptr camera);
  void render(std::vector<Light::Ptr> lights, Scene::Ptr scene, Camera::Ptr camera );

  Material::Ptr getDepthMaterial(Object3D::Ptr object,
                            Material::Ptr material,
                            bool isPointLight,
                            const math::Vector3 &lightPositionWorld,
                            float shadowCameraNear,
                            float shadowCameraFar );

  void renderObject(Object3D::Ptr object, Camera::Ptr camera, Camera::Ptr shadowCamera, bool isPointLight);

  ShadowMapType type() const {return _type;}

  void setType(ShadowMapType type) {_type = type;}
};

}
}


#endif //THREEPP_SHADOWMAP_H
