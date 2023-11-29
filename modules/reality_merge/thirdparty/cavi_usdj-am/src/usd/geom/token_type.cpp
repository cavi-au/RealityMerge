/**************************************************************************/
/* usd/geom/token_type.cpp                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             RealityMerge                               */
/*                          https://cavi.au.dk/                           */
/**************************************************************************/
/* Copyright (c) 2023-present RealityMerge contributors (see AUTHORS.md). */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>

// local
#include "detail/enum_string.hpp"
#include "usd/geom/token_type.hpp"

namespace {

using cavi::usdj_am::usd::geom::TokenType;

static std::map<std::string_view, TokenType> const TAGS = {
    {"accelerations", TokenType::ACCELERATIONS},
    {"all", TokenType::ALL},
    {"angularVelocities", TokenType::ANGULAR_VELOCITIES},
    {"axis", TokenType::AXIS},
    {"basis", TokenType::BASIS},
    {"bezier", TokenType::BEZIER},
    {"bilinear", TokenType::BILINEAR},
    {"boundaries", TokenType::BOUNDARIES},
    {"bounds", TokenType::BOUNDS},
    {"box", TokenType::BOX},
    {"bspline", TokenType::BSPLINE},
    {"cards", TokenType::CARDS},
    {"catmullClark", TokenType::CATMULL_CLARK},
    {"catmullRom", TokenType::CATMULL_ROM},
    {"clippingPlanes", TokenType::CLIPPING_PLANES},
    {"clippingRange", TokenType::CLIPPING_RANGE},
    {"closed", TokenType::CLOSED},
    {"constant", TokenType::CONSTANT},
    {"cornerIndices", TokenType::CORNER_INDICES},
    {"cornerSharpnesses", TokenType::CORNER_SHARPNESSES},
    {"cornersOnly", TokenType::CORNERS_ONLY},
    {"cornersPlus1", TokenType::CORNERS_PLUS_1},
    {"cornersPlus2", TokenType::CORNERS_PLUS_2},
    {"creaseIndices", TokenType::CREASE_INDICES},
    {"creaseLengths", TokenType::CREASE_LENGTHS},
    {"creaseSharpnesses", TokenType::CREASE_SHARPNESSES},
    {"cross", TokenType::CROSS},
    {"cubic", TokenType::CUBIC},
    {"curveVertexCounts", TokenType::CURVE_VERTEX_COUNTS},
    {"default", TokenType::DEFAULT},
    {"doubleSided", TokenType::DOUBLE_SIDED},
    {"edgeAndCorner", TokenType::EDGE_AND_CORNER},
    {"edgeOnly", TokenType::EDGE_ONLY},
    {"elementSize", TokenType::ELEMENT_SIZE},
    {"elementType", TokenType::ELEMENT_TYPE},
    {"exposure", TokenType::EXPOSURE},
    {"extent", TokenType::EXTENT},
    {"extentsHint", TokenType::EXTENTS_HINT},
    {"face", TokenType::FACE},
    {"faceVarying", TokenType::FACE_VARYING},
    {"faceVaryingLinearInterpolation", TokenType::FACE_VARYING_LINEAR_INTERPOLATION},
    {"faceVertexCounts", TokenType::FACE_VERTEX_COUNTS},
    {"faceVertexIndices", TokenType::FACE_VERTEX_INDICES},
    {"familyName", TokenType::FAMILY_NAME},
    {"focalLength", TokenType::FOCAL_LENGTH},
    {"focusDistance", TokenType::FOCUS_DISTANCE},
    {"fromTexture", TokenType::FROM_TEXTURE},
    {"fStop", TokenType::F_STOP},
    {"guide", TokenType::GUIDE},
    {"guideVisibility", TokenType::GUIDE_VISIBILITY},
    {"height", TokenType::HEIGHT},
    {"holeIndices", TokenType::HOLE_INDICES},
    {"horizontalAperture", TokenType::HORIZONTAL_APERTURE},
    {"horizontalApertureOffset", TokenType::HORIZONTAL_APERTURE_OFFSET},
    {"ids", TokenType::IDS},
    {"inactiveIds", TokenType::INACTIVE_IDS},
    {"indices", TokenType::INDICES},
    {"inherited", TokenType::INHERITED},
    {"interpolateBoundary", TokenType::INTERPOLATE_BOUNDARY},
    {"interpolation", TokenType::INTERPOLATION},
    {"invisible", TokenType::INVISIBLE},
    {"invisibleIds", TokenType::INVISIBLE_IDS},
    {"knots", TokenType::KNOTS},
    {"left", TokenType::LEFT},
    {"leftHanded", TokenType::LEFT_HANDED},
    {"length", TokenType::LENGTH},
    {"linear", TokenType::LINEAR},
    {"loop", TokenType::LOOP},
    {"metersPerUnit", TokenType::METERS_PER_UNIT},
    {"model:applyDrawMode", TokenType::MODEL_APPLY_DRAW_MODE},
    {"model:cardGeometry", TokenType::MODEL_CARD_GEOMETRY},
    {"model:cardTextureXNeg", TokenType::MODEL_CARD_TEXTURE_X_NEG},
    {"model:cardTextureXPos", TokenType::MODEL_CARD_TEXTURE_X_POS},
    {"model:cardTextureYNeg", TokenType::MODEL_CARD_TEXTURE_Y_NEG},
    {"model:cardTextureYPos", TokenType::MODEL_CARD_TEXTURE_Y_POS},
    {"model:cardTextureZNeg", TokenType::MODEL_CARD_TEXTURE_Z_NEG},
    {"model:cardTextureZPos", TokenType::MODEL_CARD_TEXTURE_Z_POS},
    {"model:drawMode", TokenType::MODEL_DRAW_MODE},
    {"model:drawModeColor", TokenType::MODEL_DRAW_MODE_COLOR},
    {"mono", TokenType::MONO},
    {"motion:blurScale", TokenType::MOTION_BLUR_SCALE},
    {"motion:nonlinearSampleCount", TokenType::MOTION_NONLINEAR_SAMPLE_COUNT},
    {"motion:velocityScale", TokenType::MOTION_VELOCITY_SCALE},
    {"none", TokenType::NONE},
    {"nonOverlapping", TokenType::NON_OVERLAPPING},
    {"nonperiodic", TokenType::NON_PERIODIC},
    {"normals", TokenType::NORMALS},
    {"open", TokenType::OPEN},
    {"order", TokenType::ORDER},
    {"orientation", TokenType::ORIENTATION},
    {"orientations", TokenType::ORIENTATIONS},
    {"origin", TokenType::ORIGIN},
    {"orthographic", TokenType::ORTHOGRAPHIC},
    {"partition", TokenType::PARTITION},
    {"periodic", TokenType::PERIODIC},
    {"perspective", TokenType::PERSPECTIVE},
    {"pinned", TokenType::PINNED},
    {"pivot", TokenType::PIVOT},
    {"points", TokenType::POINTS},
    {"pointWeights", TokenType::POINT_WEIGHTS},
    {"positions", TokenType::POSITIONS},
    {"primvars:displayColor", TokenType::PRIMVARS_DISPLAY_COLOR},
    {"primvars:displayOpacity", TokenType::PRIMVARS_DISPLAY_OPACITY},
    {"projection", TokenType::PROJECTION},
    {"protoIndices", TokenType::PROTO_INDICES},
    {"prototypes", TokenType::PROTOTYPES},
    {"proxy", TokenType::PROXY},
    {"proxyPrim", TokenType::PROXY_PRIM},
    {"proxyVisibility", TokenType::PROXY_VISIBILITY},
    {"purpose", TokenType::PURPOSE},
    {"radius", TokenType::RADIUS},
    {"ranges", TokenType::RANGES},
    {"render", TokenType::RENDER},
    {"renderVisibility", TokenType::RENDER_VISIBILITY},
    {"right", TokenType::RIGHT},
    {"rightHanded", TokenType::RIGHT_HANDED},
    {"scales", TokenType::SCALES},
    {"shutter:close", TokenType::SHUTTER_CLOSE},
    {"shutter:open", TokenType::SHUTTER_OPEN},
    {"size", TokenType::SIZE},
    {"smooth", TokenType::SMOOTH},
    {"stereoRole", TokenType::STEREO_ROLE},
    {"subdivisionScheme", TokenType::SUBDIVISION_SCHEME},
    {"tangents", TokenType::TANGENTS},
    {"triangleSubdivisionRule", TokenType::TRIANGLE_SUBDIVISION_RULE},
    {"trimCurve:counts", TokenType::TRIM_CURVE_COUNTS},
    {"trimCurve:knots", TokenType::TRIM_CURVE_KNOTS},
    {"trimCurve:orders", TokenType::TRIM_CURVE_ORDERS},
    {"trimCurve:points", TokenType::TRIM_CURVE_POINTS},
    {"trimCurve:ranges", TokenType::TRIM_CURVE_RANGES},
    {"trimCurve:vertexCounts", TokenType::TRIM_CURVE_VERTEX_COUNTS},
    {"type", TokenType::TYPE},
    {"uForm", TokenType::U_FORM},
    {"uKnots", TokenType::U_KNOTS},
    {"unauthoredValuesIndex", TokenType::UNAUTHORED_VALUES_INDEX},
    {"uniform", TokenType::UNIFORM},
    {"unrestricted", TokenType::UNRESTRICTED},
    {"uOrder", TokenType::U_ORDER},
    {"upAxis", TokenType::UP_AXIS},
    {"uRange", TokenType::U_RANGE},
    {"uVertexCount", TokenType::U_VERTEX_COUNT},
    {"varying", TokenType::VARYING},
    {"velocities", TokenType::VELOCITIES},
    {"vertex", TokenType::VERTEX},
    {"verticalAperture", TokenType::VERTICAL_APERTURE},
    {"verticalApertureOffset", TokenType::VERTICAL_APERTURE_OFFSET},
    {"vForm", TokenType::V_FORM},
    {"visibility", TokenType::VISIBILITY},
    {"visible", TokenType::VISIBLE},
    {"vKnots", TokenType::V_KNOTS},
    {"vOrder", TokenType::V_ORDER},
    {"vRange", TokenType::V_RANGE},
    {"vVertexCount", TokenType::V_VERTEX_COUNT},
    {"width", TokenType::WIDTH},
    {"widths", TokenType::WIDTHS},
    {"wrap", TokenType::WRAP},
    {"X", TokenType::X},
    {"xformOpOrder", TokenType::XFORM_OP_ORDER},
    {"Y", TokenType::Y},
    {"Z", TokenType::Z},
    {"BasisCurves", TokenType::BASIS_CURVES},
    {"Boundable", TokenType::BOUNDABLE},
    {"Camera", TokenType::CAMERA},
    {"Capsule", TokenType::CAPSULE},
    {"Cone", TokenType::CONE},
    {"Cube", TokenType::CUBE},
    {"Curves", TokenType::CURVES},
    {"Cylinder", TokenType::CYLINDER},
    {"GeomModelAPI", TokenType::GEOM_MODEL_API},
    {"GeomSubset", TokenType::GEOM_SUBSET},
    {"Gprim", TokenType::GPRIM},
    {"HermiteCurves", TokenType::HERMITE_CURVES},
    {"Imageable", TokenType::IMAGEABLE},
    {"Mesh", TokenType::MESH},
    {"MotionAPI", TokenType::MOTION_API},
    {"NurbsCurves", TokenType::NURBS_CURVES},
    {"NurbsPatch", TokenType::NURBS_PATCH},
    {"Plane", TokenType::PLANE},
    {"PointBased", TokenType::POINT_BASED},
    {"PointInstancer", TokenType::POINT_INSTANCER},
    {"Points", TokenType::POINTS_},
    {"PrimvarsAPI", TokenType::PRIMVARS_API},
    {"Scope", TokenType::SCOPE},
    {"Sphere", TokenType::SPHERE},
    {"VisibilityAPI", TokenType::VISIBILITY_API},
    {"Xform", TokenType::XFORM},
    {"Xformable", TokenType::XFORMABLE},
    {"XformCommonAPI", TokenType::XFORM_COMMON_API}};

}  // namespace

namespace cavi {
namespace usdj_am {
namespace usd {
namespace geom {

std::optional<TokenType> extract_TokenType(std::string_view const& view) {
    return detail::extract_enum_tag<TokenType>(TAGS, view);
}

TokenType to_TokenType(std::string_view const& view) {
    auto const result = detail::extract_enum_tag<TokenType>(TAGS, view);
    if (!result) {
        std::ostringstream what;
        what << __func__ << "(view == \"" << view << "\")";
        throw std::invalid_argument(what.str());
    }
    return *result;
}

std::istream& operator>>(std::istream& is, TokenType& out) {
    return detail::operator>><TokenType>(is, std::make_pair(std::cref(TAGS), std::ref(out)));
}

std::ostream& operator<<(std::ostream& os, TokenType const& in) {
    os << detail::extract_enum_string<TokenType>(TAGS, in).value_or("???");
    return os;
}

}  // namespace geom
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi
