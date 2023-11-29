/**************************************************************************/
/* usd/geom/token_type.hpp                                                */
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

#ifndef CAVI_USDJ_AM_USD_GEOM_TOKEN_TYPE_HPP
#define CAVI_USDJ_AM_USD_GEOM_TOKEN_TYPE_HPP

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string_view>

// boost::python::class_<UsdGeomTokensType, boost::noncopyable>
//     cls("Tokens", boost::python::no_init);
// _AddToken(cls, "accelerations", UsdGeomTokens->accelerations);
// _AddToken(cls, "all", UsdGeomTokens->all);
// _AddToken(cls, "angularVelocities", UsdGeomTokens->angularVelocities);
// _AddToken(cls, "axis", UsdGeomTokens->axis);
// _AddToken(cls, "basis", UsdGeomTokens->basis);
// _AddToken(cls, "bezier", UsdGeomTokens->bezier);
// _AddToken(cls, "bilinear", UsdGeomTokens->bilinear);
// _AddToken(cls, "boundaries", UsdGeomTokens->boundaries);
// _AddToken(cls, "bounds", UsdGeomTokens->bounds);
// _AddToken(cls, "box", UsdGeomTokens->box);
// _AddToken(cls, "bspline", UsdGeomTokens->bspline);
// _AddToken(cls, "cards", UsdGeomTokens->cards);
// _AddToken(cls, "catmullClark", UsdGeomTokens->catmullClark);
// _AddToken(cls, "catmullRom", UsdGeomTokens->catmullRom);
// _AddToken(cls, "clippingPlanes", UsdGeomTokens->clippingPlanes);
// _AddToken(cls, "clippingRange", UsdGeomTokens->clippingRange);
// _AddToken(cls, "closed", UsdGeomTokens->closed);
// _AddToken(cls, "constant", UsdGeomTokens->constant);
// _AddToken(cls, "cornerIndices", UsdGeomTokens->cornerIndices);
// _AddToken(cls, "cornerSharpnesses", UsdGeomTokens->cornerSharpnesses);
// _AddToken(cls, "cornersOnly", UsdGeomTokens->cornersOnly);
// _AddToken(cls, "cornersPlus1", UsdGeomTokens->cornersPlus1);
// _AddToken(cls, "cornersPlus2", UsdGeomTokens->cornersPlus2);
// _AddToken(cls, "creaseIndices", UsdGeomTokens->creaseIndices);
// _AddToken(cls, "creaseLengths", UsdGeomTokens->creaseLengths);
// _AddToken(cls, "creaseSharpnesses", UsdGeomTokens->creaseSharpnesses);
// _AddToken(cls, "cross", UsdGeomTokens->cross);
// _AddToken(cls, "cubic", UsdGeomTokens->cubic);
// _AddToken(cls, "curveVertexCounts", UsdGeomTokens->curveVertexCounts);
// _AddToken(cls, "default_", UsdGeomTokens->default_);
// _AddToken(cls, "doubleSided", UsdGeomTokens->doubleSided);
// _AddToken(cls, "edgeAndCorner", UsdGeomTokens->edgeAndCorner);
// _AddToken(cls, "edgeOnly", UsdGeomTokens->edgeOnly);
// _AddToken(cls, "elementSize", UsdGeomTokens->elementSize);
// _AddToken(cls, "elementType", UsdGeomTokens->elementType);
// _AddToken(cls, "exposure", UsdGeomTokens->exposure);
// _AddToken(cls, "extent", UsdGeomTokens->extent);
// _AddToken(cls, "extentsHint", UsdGeomTokens->extentsHint);
// _AddToken(cls, "face", UsdGeomTokens->face);
// _AddToken(cls, "faceVarying", UsdGeomTokens->faceVarying);
// _AddToken(cls, "faceVaryingLinearInterpolation", UsdGeomTokens->faceVaryingLinearInterpolation);
// _AddToken(cls, "faceVertexCounts", UsdGeomTokens->faceVertexCounts);
// _AddToken(cls, "faceVertexIndices", UsdGeomTokens->faceVertexIndices);
// _AddToken(cls, "familyName", UsdGeomTokens->familyName);
// _AddToken(cls, "focalLength", UsdGeomTokens->focalLength);
// _AddToken(cls, "focusDistance", UsdGeomTokens->focusDistance);
// _AddToken(cls, "fromTexture", UsdGeomTokens->fromTexture);
// _AddToken(cls, "fStop", UsdGeomTokens->fStop);
// _AddToken(cls, "guide", UsdGeomTokens->guide);
// _AddToken(cls, "guideVisibility", UsdGeomTokens->guideVisibility);
// _AddToken(cls, "height", UsdGeomTokens->height);
// _AddToken(cls, "hermite", UsdGeomTokens->hermite);
// _AddToken(cls, "holeIndices", UsdGeomTokens->holeIndices);
// _AddToken(cls, "horizontalAperture", UsdGeomTokens->horizontalAperture);
// _AddToken(cls, "horizontalApertureOffset", UsdGeomTokens->horizontalApertureOffset);
// _AddToken(cls, "ids", UsdGeomTokens->ids);
// _AddToken(cls, "inactiveIds", UsdGeomTokens->inactiveIds);
// _AddToken(cls, "indices", UsdGeomTokens->indices);
// _AddToken(cls, "inherited", UsdGeomTokens->inherited);
// _AddToken(cls, "interpolateBoundary", UsdGeomTokens->interpolateBoundary);
// _AddToken(cls, "interpolation", UsdGeomTokens->interpolation);
// _AddToken(cls, "invisible", UsdGeomTokens->invisible);
// _AddToken(cls, "invisibleIds", UsdGeomTokens->invisibleIds);
// _AddToken(cls, "knots", UsdGeomTokens->knots);
// _AddToken(cls, "left", UsdGeomTokens->left);
// _AddToken(cls, "leftHanded", UsdGeomTokens->leftHanded);
// _AddToken(cls, "length", UsdGeomTokens->length);
// _AddToken(cls, "linear", UsdGeomTokens->linear);
// _AddToken(cls, "loop", UsdGeomTokens->loop);
// _AddToken(cls, "metersPerUnit", UsdGeomTokens->metersPerUnit);
// _AddToken(cls, "modelApplyDrawMode", UsdGeomTokens->modelApplyDrawMode);
// _AddToken(cls, "modelCardGeometry", UsdGeomTokens->modelCardGeometry);
// _AddToken(cls, "modelCardTextureXNeg", UsdGeomTokens->modelCardTextureXNeg);
// _AddToken(cls, "modelCardTextureXPos", UsdGeomTokens->modelCardTextureXPos);
// _AddToken(cls, "modelCardTextureYNeg", UsdGeomTokens->modelCardTextureYNeg);
// _AddToken(cls, "modelCardTextureYPos", UsdGeomTokens->modelCardTextureYPos);
// _AddToken(cls, "modelCardTextureZNeg", UsdGeomTokens->modelCardTextureZNeg);
// _AddToken(cls, "modelCardTextureZPos", UsdGeomTokens->modelCardTextureZPos);
// _AddToken(cls, "modelDrawMode", UsdGeomTokens->modelDrawMode);
// _AddToken(cls, "modelDrawModeColor", UsdGeomTokens->modelDrawModeColor);
// _AddToken(cls, "mono", UsdGeomTokens->mono);
// _AddToken(cls, "motionBlurScale", UsdGeomTokens->motionBlurScale);
// _AddToken(cls, "motionNonlinearSampleCount", UsdGeomTokens->motionNonlinearSampleCount);
// _AddToken(cls, "motionVelocityScale", UsdGeomTokens->motionVelocityScale);
// _AddToken(cls, "none", UsdGeomTokens->none);
// _AddToken(cls, "nonOverlapping", UsdGeomTokens->nonOverlapping);
// _AddToken(cls, "nonperiodic", UsdGeomTokens->nonperiodic);
// _AddToken(cls, "normals", UsdGeomTokens->normals);
// _AddToken(cls, "open", UsdGeomTokens->open);
// _AddToken(cls, "order", UsdGeomTokens->order);
// _AddToken(cls, "orientation", UsdGeomTokens->orientation);
// _AddToken(cls, "orientations", UsdGeomTokens->orientations);
// _AddToken(cls, "origin", UsdGeomTokens->origin);
// _AddToken(cls, "orthographic", UsdGeomTokens->orthographic);
// _AddToken(cls, "partition", UsdGeomTokens->partition);
// _AddToken(cls, "periodic", UsdGeomTokens->periodic);
// _AddToken(cls, "perspective", UsdGeomTokens->perspective);
// _AddToken(cls, "pinned", UsdGeomTokens->pinned);
// _AddToken(cls, "pivot", UsdGeomTokens->pivot);
// _AddToken(cls, "points", UsdGeomTokens->points);
// _AddToken(cls, "pointWeights", UsdGeomTokens->pointWeights);
// _AddToken(cls, "positions", UsdGeomTokens->positions);
// _AddToken(cls, "power", UsdGeomTokens->power);
// _AddToken(cls, "primvarsDisplayColor", UsdGeomTokens->primvarsDisplayColor);
// _AddToken(cls, "primvarsDisplayOpacity", UsdGeomTokens->primvarsDisplayOpacity);
// _AddToken(cls, "projection", UsdGeomTokens->projection);
// _AddToken(cls, "protoIndices", UsdGeomTokens->protoIndices);
// _AddToken(cls, "prototypes", UsdGeomTokens->prototypes);
// _AddToken(cls, "proxy", UsdGeomTokens->proxy);
// _AddToken(cls, "proxyPrim", UsdGeomTokens->proxyPrim);
// _AddToken(cls, "proxyVisibility", UsdGeomTokens->proxyVisibility);
// _AddToken(cls, "purpose", UsdGeomTokens->purpose);
// _AddToken(cls, "radius", UsdGeomTokens->radius);
// _AddToken(cls, "ranges", UsdGeomTokens->ranges);
// _AddToken(cls, "render", UsdGeomTokens->render);
// _AddToken(cls, "renderVisibility", UsdGeomTokens->renderVisibility);
// _AddToken(cls, "right", UsdGeomTokens->right);
// _AddToken(cls, "rightHanded", UsdGeomTokens->rightHanded);
// _AddToken(cls, "scales", UsdGeomTokens->scales);
// _AddToken(cls, "shutterClose", UsdGeomTokens->shutterClose);
// _AddToken(cls, "shutterOpen", UsdGeomTokens->shutterOpen);
// _AddToken(cls, "size", UsdGeomTokens->size);
// _AddToken(cls, "smooth", UsdGeomTokens->smooth);
// _AddToken(cls, "stereoRole", UsdGeomTokens->stereoRole);
// _AddToken(cls, "subdivisionScheme", UsdGeomTokens->subdivisionScheme);
// _AddToken(cls, "tangents", UsdGeomTokens->tangents);
// _AddToken(cls, "triangleSubdivisionRule", UsdGeomTokens->triangleSubdivisionRule);
// _AddToken(cls, "trimCurveCounts", UsdGeomTokens->trimCurveCounts);
// _AddToken(cls, "trimCurveKnots", UsdGeomTokens->trimCurveKnots);
// _AddToken(cls, "trimCurveOrders", UsdGeomTokens->trimCurveOrders);
// _AddToken(cls, "trimCurvePoints", UsdGeomTokens->trimCurvePoints);
// _AddToken(cls, "trimCurveRanges", UsdGeomTokens->trimCurveRanges);
// _AddToken(cls, "trimCurveVertexCounts", UsdGeomTokens->trimCurveVertexCounts);
// _AddToken(cls, "type", UsdGeomTokens->type);
// _AddToken(cls, "uForm", UsdGeomTokens->uForm);
// _AddToken(cls, "uKnots", UsdGeomTokens->uKnots);
// _AddToken(cls, "unauthoredValuesIndex", UsdGeomTokens->unauthoredValuesIndex);
// _AddToken(cls, "uniform", UsdGeomTokens->uniform);
// _AddToken(cls, "unrestricted", UsdGeomTokens->unrestricted);
// _AddToken(cls, "uOrder", UsdGeomTokens->uOrder);
// _AddToken(cls, "upAxis", UsdGeomTokens->upAxis);
// _AddToken(cls, "uRange", UsdGeomTokens->uRange);
// _AddToken(cls, "uVertexCount", UsdGeomTokens->uVertexCount);
// _AddToken(cls, "varying", UsdGeomTokens->varying);
// _AddToken(cls, "velocities", UsdGeomTokens->velocities);
// _AddToken(cls, "vertex", UsdGeomTokens->vertex);
// _AddToken(cls, "verticalAperture", UsdGeomTokens->verticalAperture);
// _AddToken(cls, "verticalApertureOffset", UsdGeomTokens->verticalApertureOffset);
// _AddToken(cls, "vForm", UsdGeomTokens->vForm);
// _AddToken(cls, "visibility", UsdGeomTokens->visibility);
// _AddToken(cls, "visible", UsdGeomTokens->visible);
// _AddToken(cls, "vKnots", UsdGeomTokens->vKnots);
// _AddToken(cls, "vOrder", UsdGeomTokens->vOrder);
// _AddToken(cls, "vRange", UsdGeomTokens->vRange);
// _AddToken(cls, "vVertexCount", UsdGeomTokens->vVertexCount);
// _AddToken(cls, "width", UsdGeomTokens->width);
// _AddToken(cls, "widths", UsdGeomTokens->widths);
// _AddToken(cls, "wrap", UsdGeomTokens->wrap);
// _AddToken(cls, "x", UsdGeomTokens->x);
// _AddToken(cls, "xformOpOrder", UsdGeomTokens->xformOpOrder);
// _AddToken(cls, "y", UsdGeomTokens->y);
// _AddToken(cls, "z", UsdGeomTokens->z);
// _AddToken(cls, "BasisCurves", UsdGeomTokens->BasisCurves);
// _AddToken(cls, "Boundable", UsdGeomTokens->Boundable);
// _AddToken(cls, "Camera", UsdGeomTokens->Camera);
// _AddToken(cls, "Capsule", UsdGeomTokens->Capsule);
// _AddToken(cls, "Cone", UsdGeomTokens->Cone);
// _AddToken(cls, "Cube", UsdGeomTokens->Cube);
// _AddToken(cls, "Curves", UsdGeomTokens->Curves);
// _AddToken(cls, "Cylinder", UsdGeomTokens->Cylinder);
// _AddToken(cls, "GeomModelAPI", UsdGeomTokens->GeomModelAPI);
// _AddToken(cls, "GeomSubset", UsdGeomTokens->GeomSubset);
// _AddToken(cls, "Gprim", UsdGeomTokens->Gprim);
// _AddToken(cls, "HermiteCurves", UsdGeomTokens->HermiteCurves);
// _AddToken(cls, "Imageable", UsdGeomTokens->Imageable);
// _AddToken(cls, "Mesh", UsdGeomTokens->Mesh);
// _AddToken(cls, "MotionAPI", UsdGeomTokens->MotionAPI);
// _AddToken(cls, "NurbsCurves", UsdGeomTokens->NurbsCurves);
// _AddToken(cls, "NurbsPatch", UsdGeomTokens->NurbsPatch);
// _AddToken(cls, "Plane", UsdGeomTokens->Plane);
// _AddToken(cls, "PointBased", UsdGeomTokens->PointBased);
// _AddToken(cls, "PointInstancer", UsdGeomTokens->PointInstancer);
// _AddToken(cls, "Points", UsdGeomTokens->Points);
// _AddToken(cls, "PrimvarsAPI", UsdGeomTokens->PrimvarsAPI);
// _AddToken(cls, "Scope", UsdGeomTokens->Scope);
// _AddToken(cls, "Sphere", UsdGeomTokens->Sphere);
// _AddToken(cls, "VisibilityAPI", UsdGeomTokens->VisibilityAPI);
// _AddToken(cls, "Xform", UsdGeomTokens->Xform);
// _AddToken(cls, "Xformable", UsdGeomTokens->Xformable);
// _AddToken(cls, "XformCommonAPI", UsdGeomTokens->XformCommonAPI);

namespace cavi {
namespace usdj_am {
namespace usd {
namespace geom {

/// \brief An enum representing a `UsdGeomTokensType` tag stored as a
///        string within a "USDA_Declaration" node in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
enum class TokenType : std::uint8_t {
    BEGIN__ = 1,
    ACCELERATIONS = BEGIN__,
    ALL,
    ANGULAR_VELOCITIES,
    AXIS,
    BASIS,
    BEZIER,
    BILINEAR,
    BOUNDARIES,
    BOUNDS,
    BOX,
    BSPLINE,
    CARDS,
    CATMULL_CLARK,
    CATMULL_ROM,
    CLIPPING_PLANES,
    CLIPPING_RANGE,
    CLOSED,
    CONSTANT,
    CORNER_INDICES,
    CORNER_SHARPNESSES,
    CORNERS_ONLY,
    CORNERS_PLUS_1,
    CORNERS_PLUS_2,
    CREASE_INDICES,
    CREASE_LENGTHS,
    CREASE_SHARPNESSES,
    CROSS,
    CUBIC,
    CURVE_VERTEX_COUNTS,
    DEFAULT,
    DOUBLE_SIDED,
    EDGE_AND_CORNER,
    EDGE_ONLY,
    ELEMENT_SIZE,
    ELEMENT_TYPE,
    EXPOSURE,
    EXTENT,
    EXTENTS_HINT,
    FACE,
    FACE_VARYING,
    FACE_VARYING_LINEAR_INTERPOLATION,
    FACE_VERTEX_COUNTS,
    FACE_VERTEX_INDICES,
    FAMILY_NAME,
    FOCAL_LENGTH,
    FOCUS_DISTANCE,
    FROM_TEXTURE,
    F_STOP,
    GUIDE,
    GUIDE_VISIBILITY,
    HEIGHT,
    HOLE_INDICES,
    HORIZONTAL_APERTURE,
    HORIZONTAL_APERTURE_OFFSET,
    IDS,
    INACTIVE_IDS,
    INDICES,
    INHERITED,
    INTERPOLATE_BOUNDARY,
    INTERPOLATION,
    INVISIBLE,
    INVISIBLE_IDS,
    KNOTS,
    LEFT,
    LEFT_HANDED,
    LENGTH,
    LINEAR,
    LOOP,
    METERS_PER_UNIT,
    MODEL_APPLY_DRAW_MODE,
    MODEL_CARD_GEOMETRY,
    MODEL_CARD_TEXTURE_X_NEG,
    MODEL_CARD_TEXTURE_X_POS,
    MODEL_CARD_TEXTURE_Y_NEG,
    MODEL_CARD_TEXTURE_Y_POS,
    MODEL_CARD_TEXTURE_Z_NEG,
    MODEL_CARD_TEXTURE_Z_POS,
    MODEL_DRAW_MODE,
    MODEL_DRAW_MODE_COLOR,
    MONO,
    MOTION_BLUR_SCALE,
    MOTION_NONLINEAR_SAMPLE_COUNT,
    MOTION_VELOCITY_SCALE,
    NONE,
    NON_OVERLAPPING,
    NON_PERIODIC,
    NORMALS,
    OPEN,
    ORDER,
    ORIENTATION,
    ORIENTATIONS,
    ORIGIN,
    ORTHOGRAPHIC,
    PARTITION,
    PERIODIC,
    PERSPECTIVE,
    PINNED,
    PIVOT,
    /// The primary geometry attribute for all `PointBased` primitives,
    /// describes points in (local) space.
    POINTS,
    POINT_WEIGHTS,
    POSITIONS,
    PRIMVARS_DISPLAY_COLOR,
    PRIMVARS_DISPLAY_OPACITY,
    PROJECTION,
    PROTO_INDICES,
    PROTOTYPES,
    PROXY,
    PROXY_PRIM,
    PROXY_VISIBILITY,
    PURPOSE,
    RADIUS,
    RANGES,
    RENDER,
    RENDER_VISIBILITY,
    RIGHT,
    RIGHT_HANDED,
    SCALES,
    SHUTTER_CLOSE,
    SHUTTER_OPEN,
    SIZE,
    SMOOTH,
    STEREO_ROLE,
    SUBDIVISION_SCHEME,
    TANGENTS,
    TRIANGLE_SUBDIVISION_RULE,
    TRIM_CURVE_COUNTS,
    TRIM_CURVE_KNOTS,
    TRIM_CURVE_ORDERS,
    TRIM_CURVE_POINTS,
    TRIM_CURVE_RANGES,
    TRIM_CURVE_VERTEX_COUNTS,
    TYPE,
    U_FORM,
    U_KNOTS,
    UNAUTHORED_VALUES_INDEX,
    UNIFORM,
    UNRESTRICTED,
    U_ORDER,
    UP_AXIS,
    U_RANGE,
    U_VERTEX_COUNT,
    VARYING,
    VELOCITIES,
    VERTEX,
    VERTICAL_APERTURE,
    VERTICAL_APERTURE_OFFSET,
    V_FORM,
    VISIBILITY,
    VISIBLE,
    V_KNOTS,
    V_ORDER,
    V_RANGE,
    V_VERTEX_COUNT,
    WIDTH,
    WIDTHS,
    WRAP,
    X,
    XFORM_OP_ORDER,
    Y,
    Z,
    BASIS_CURVES,
    BOUNDABLE,
    CAMERA,
    CAPSULE,
    CONE,
    CUBE,
    CURVES,
    CYLINDER,
    GEOM_MODEL_API,
    GEOM_SUBSET,
    GPRIM,
    HERMITE_CURVES,
    IMAGEABLE,
    MESH,
    MOTION_API,
    NURBS_CURVES,
    NURBS_PATCH,
    PLANE,
    POINT_BASED,
    POINT_INSTANCER,
    /// Schema identifer and family for `UsdGeomPoints`.
    POINTS_,
    PRIMVARS_API,
    SCOPE,
    SPHERE,
    VISIBILITY_API,
    XFORM,
    XFORMABLE,
    XFORM_COMMON_API,
    END__,
    SIZE__ = END__ - BEGIN__
};

/// \brief Extracts a `TokenType` enum tag from a string view.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `TokenType` enum tag whose serialized form is \p view or
///          `std::nullopt`.
std::optional<TokenType> extract_TokenType(std::string_view const& view);

/// \brief Converts a string view into a `TokenType` enum tag.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `TokenType` enum tag whose serialized form is \p view.
/// \throws std::invalid_argument
TokenType to_TokenType(std::string_view const& view);

std::istream& operator>>(std::istream& is, TokenType& out);

std::ostream& operator<<(std::ostream& os, TokenType const& in);

}  // namespace geom
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_USD_GEOM_TOKEN_TYPE_HPP
