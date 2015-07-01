uniform float focusDistance;
uniform float aperture;

const float baseArea = .5 * .5; // area of 1 pixel diameter point

const float minPointSize = 1.;
const float maxPointSize = 40.;

void main() {
	// get the homogeneous 2d position
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	// use the distance from the camera and aperture to determine the radius
	// the +1 is because point diameters <1 are rendered differently than those >1
  gl_PointSize = clamp(abs(gl_Position.z - focusDistance) * aperture, minPointSize, maxPointSize);

	gl_FrontColor = gl_Color;
  float radius = gl_PointSize / 2.;
	// divide the color alpha by the area
  gl_FrontColor.a /= radius * radius / baseArea;
}
