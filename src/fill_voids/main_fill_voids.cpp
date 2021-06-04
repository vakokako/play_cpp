#include "fill_voids.hpp"

#include <xtensor/xtensor.hpp>

int main()
{
    const size_t cSize = 512;

    xt::xtensor<uint8_t, 3> vLabels({cSize, cSize, cSize});

    // // modifies labels as a side effect, returns number of voxels filled in
    // size_t fill_ct = fill_voids::binary_fill_holes<uint8_t>(vLabels.data(), cSize, cSize, cSize);

    // let labels now represent a 512x512 2D image
    size_t fill_ct = fill_voids::binary_fill_holes<uint8_t>(vLabels.data(), cSize, cSize);
}