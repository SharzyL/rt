#include "photon_mapping.h"

namespace RT {

PhotonMappingRender::PhotonMappingRender(int sub_pixel, int sub_sample, float gamma): sub_pixel(sub_pixel), sub_sample(sub_sample), gamma(gamma) {

}

void PhotonMappingRender::Render(const Object3D &obj, const Camera &camera, const std::string &output_file) {

}

} // namespace RT