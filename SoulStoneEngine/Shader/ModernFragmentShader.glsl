#version 400
const int MAX_LIGHT_NUM = 16;
uniform vec3 u_lightPositions[MAX_LIGHT_NUM];
uniform vec4 u_lightColorAndBrightness[MAX_LIGHT_NUM];
uniform float u_lightInnerApertureDot[MAX_LIGHT_NUM];
uniform float u_lightOuterApertureDot[MAX_LIGHT_NUM];
uniform float u_lightInnerRadius[MAX_LIGHT_NUM];
uniform float u_lightOuterRadius[MAX_LIGHT_NUM];
uniform float u_lightAmbientness[MAX_LIGHT_NUM];
uniform vec3 u_lightForwardDirection[MAX_LIGHT_NUM];
uniform int u_isDirectionalLight[MAX_LIGHT_NUM];

uniform sampler3D Noise3;

uniform int enableOcclusionParallax;
uniform int enableParallax;

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_emissiveTexture;
uniform sampler2D u_specularTexture;
uniform sampler2D u_depthTexture;

uniform vec3 u_lightPosition;
uniform float u_time;
uniform vec3 u_cameraWorldPosition;
uniform float u_deltaTime;

//fog
uniform float u_fogStartDistance;
uniform float u_fogEndDistance;
uniform vec4 u_fogColor;

//paralax
uniform vec2 u_scaleBias;

uniform int u_isNormalMapOn;

in vec3 v_worldPosition;
in vec2 v_textureCoords;
in vec4 v_color;
in vec3 v_normal;
in mat4 v_tangentToWorldMatrix;
in mat4 v_worldToTangentMatrix;
in float v_noise;

out vec4 o_fragColor;

float SmoothStep( float value )
{
	return ( 3.0 * value * value - 2.0 * value * value * value );
}

int LFSR_Rand_Gen(in int n)
{
  // <<, ^ and & require GL_EXT_gpu_shader4.
  n = (n << 13) ^ n; 
  return (n * (n*n*15731+789221) + 1376312589) & 0x7fffffff;
}

float LFSR_Rand_Gen_f( in int n )
{
  return float(LFSR_Rand_Gen(n));
}

float noise3f(in vec3 p)
{
  ivec3 ip = ivec3(floor(p));
  vec3 u = fract(p);
  u = u*u*(3.0-2.0*u);

  int n = ip.x + ip.y*57 + ip.z*113;

  float res = mix(mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*0)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*0)),u.x),u.y),
                 mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*1)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*1)),u.x),u.y),u.z);

  return 1.0 - res*(1.0/1073741824.0);
}

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
} 

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec3 P)
{
  vec3 Pi0 = floor(P); // Integer part for indexing
  vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}

// Classic Perlin noise, periodic variant
float pnoise(vec3 P, vec3 rep)
{
  vec3 Pi0 = mod(floor(P), rep); // Integer part, modulo period
  vec3 Pi1 = mod(Pi0 + vec3(1.0), rep); // Integer part + 1, mod period
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}

float snoise(vec3 v)
{ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
}

vec2 ParallaxOcclusionRayTrace( float zStep, vec2 texStep, int sampleCount )
{
	int maxSampleIndex = sampleCount;
	int sampleIndex = 0;
	vec2 currentTexOffset;
	vec2 prevTexOffset;
	vec2 finalTexOffset;
	float currentRayZ = 1.0 - zStep;
	float prevRayZ = 1.0;
	float currentHeight = 0;
	float prevHeight = 0;

	for( sampleIndex = 0; sampleIndex < maxSampleIndex; sampleIndex++ )
	{
		currentHeight = texture2D( u_depthTexture, v_textureCoords  + currentTexOffset ).r;

		if( currentHeight > currentRayZ )
		{
			float t = ( prevHeight - prevRayZ )/ ( prevHeight - currentHeight + currentRayZ - prevRayZ );

			finalTexOffset = prevTexOffset + t * texStep;

			return v_textureCoords + finalTexOffset;
		}
		else
		{
			prevTexOffset = currentTexOffset;
			prevRayZ = currentRayZ;
			prevHeight = currentHeight;

			currentTexOffset += texStep;

			currentRayZ -= zStep;
		}
	}

	return v_textureCoords + finalTexOffset;
}

float SelfShadow( float zStep, vec2 texStep, int sampleCount, vec2 texCoords, float currentRayZ)
{
	int maxSampleIndex = sampleCount;
	int sampleIndex = 0;
	vec2 currentTexOffset;
	float shadowFactor = 1.0;
	float currentHeight;

	for( sampleIndex = 0; sampleIndex < maxSampleIndex; sampleIndex++ )
	{
		currentHeight = texture2D( u_depthTexture, texCoords + currentTexOffset ).r;

		if( currentHeight > currentRayZ )
		{
			shadowFactor = 0.3;
			return shadowFactor;
		}
		else
		{
			currentTexOffset += texStep;
			currentRayZ += zStep;
		}
	}
	return shadowFactor;
}

void main()
{
	vec2 texCoords;
	float depth = 1.0;
	float heightScale = 0.1;
	int maxSampleCount = 64;
	int minSampleCount = 8;
	float shadowFactor = 1.0;
	vec3 pixelToCamera = u_cameraWorldPosition - v_worldPosition;
	vec3 surfaceNormal = normalize( v_normal );

	float distPixToCam = length( pixelToCamera );
	vec3 pixToCamNormalize = normalize( pixelToCamera );
	vec3 pixToCamInTangentSpace = ( v_worldToTangentMatrix * vec4( pixToCamNormalize, 0.0 ) ).xyz;

	texCoords = v_textureCoords;

	//simple parallax--------------------------------------------------------------------------------------------
	if( enableParallax == 1 )
	{
		float height = texture2D( u_depthTexture, v_textureCoords ).r;
		float v = height * u_scaleBias.x - u_scaleBias.y;

		vec2 parallaxTexCoords = v_textureCoords + ( pixToCamInTangentSpace.xy * v );
		texCoords = parallaxTexCoords;
	}
	//-----------------------------------------------------------------------------------------------------------

	//Occlusion Parallax-----------------------------------------------------------------------------------------

	if( enableOcclusionParallax == 1 )
	{
		vec3 viewDirectionInTangentSpace = -pixToCamInTangentSpace;
		vec2 maxParallelOffsetVector =  -viewDirectionInTangentSpace.xy * ( heightScale / viewDirectionInTangentSpace.z );

		int sampleCount = int( mix( minSampleCount, maxSampleCount, dot( pixelToCamera, surfaceNormal ) ) );

		float zStep = 1.0 / sampleCount;
		vec2 texStep = maxParallelOffsetVector * zStep;

		texCoords = ParallaxOcclusionRayTrace( zStep, texStep, sampleCount );
		//------------------------------------------------------------------------------------------------------------

		//soft shadow-------------------------------------------------------------------------------------------------
		vec3 pixelToLight = normalize( u_lightPositions[0] - v_worldPosition );
		vec3 pixelToLightTangentSpace = ( v_worldToTangentMatrix * vec4( pixelToLight, 0.0 ) ).xyz;
		vec2 lightParallelOffsetVector = vec2( pixelToLightTangentSpace.xy ) * ( heightScale / pixelToLightTangentSpace.z );
		sampleCount = int( mix( minSampleCount, maxSampleCount, dot( pixelToLight, surfaceNormal ) ) );
		zStep = 1.0 / sampleCount;
		texStep = lightParallelOffsetVector * zStep;
		float currentHeight = texture2D( u_depthTexture, texCoords ).r + 0.1 * zStep;
		shadowFactor = SelfShadow( zStep, texStep, sampleCount, texCoords, currentHeight );
	}

	//decode normal map
	vec4 normalTexel = texture2D( u_normalTexture, texCoords );
	vec3 normalVectorInTangentSpace = ( 2.0 * normalTexel.rgb ) - 1.0;
	vec4 normalVectorInWorldSpace;
	vec4 pixelFragmentColor;

	vec4 diffuseTexel = texture2D( u_diffuseTexture, texCoords );

	if( u_isNormalMapOn == 0 )
	{
		normalVectorInWorldSpace = vec4( v_normal, 0.0 );
	}
	else
	{
		normalVectorInWorldSpace = v_tangentToWorldMatrix * vec4(normalVectorInTangentSpace, 0.0);
	}
	//Used to Calculate direct light----------------------------------------------------------------------------------------
	vec3 totalDirectLightColor; 
	vec3 displacementToLight;
	vec3 directionToPointLight;
	float distanceToLight;
	float dotOfForwardDirectionAndToPixel;
	float lightIntensityFromCone;
	float lightIntensityFromRadius;
	float lightIntensityOfNonDirectLight;
	float lightIntensityOfDirectLight;
	float lightTotalIntensity;
	vec3 lightColor;

	//Used to Calculate Specular light----------------------------------------------------------------------------------------
	vec3 cameraToPixel = v_worldPosition - u_cameraWorldPosition;
	vec3 directionCameraToPixel = normalize( cameraToPixel );
	vec4 specularTexel = texture2D( u_specularTexture, texCoords );
	float glossiness = specularTexel.b;
	float reflectivity = specularTexel.r;
	vec3 idealDirectionTolight = reflect( directionCameraToPixel , normalVectorInWorldSpace.xyz);
	vec3 totalSpecularColor;

	for(int i = 0; i < MAX_LIGHT_NUM; i++)
	{
		float ambient = u_lightAmbientness[i];
		ambient = 0;
		//Direct light------------------------------------------------------------------------------------------
		vec3 lightForwardDirection = normalize( u_lightForwardDirection[i] );
		displacementToLight = u_lightPositions[i] - v_worldPosition.xyz;
		directionToPointLight = normalize( displacementToLight );
		distanceToLight = length( displacementToLight );
		dotOfForwardDirectionAndToPixel = dot( directionToPointLight, -lightForwardDirection);
		lightIntensityFromCone = clamp( ( dotOfForwardDirectionAndToPixel - u_lightOuterApertureDot[i] ) / ( u_lightInnerApertureDot[i] - u_lightOuterApertureDot[i] ),0.0, 1.0);
		lightIntensityFromCone *= ( 1 - ambient);
		lightIntensityFromCone += ambient;
		lightIntensityFromCone = clamp ( lightIntensityFromCone, 0.0, 1.0 );

		lightIntensityFromRadius = clamp( ( u_lightOuterRadius[i] - distanceToLight  ) / ( u_lightOuterRadius[i] - u_lightInnerRadius[i] ) ,0.0,1.0);
		lightIntensityFromRadius *= ( 1 - ambient);
		lightIntensityFromRadius += ambient;
		lightIntensityFromRadius = clamp ( lightIntensityFromRadius, 0.0, 1.0 );

		lightIntensityOfNonDirectLight = clamp( dot( directionToPointLight, normalVectorInWorldSpace.xyz ), 0.0, 1.0 ) * (1.0 - u_isDirectionalLight[i]);
		lightIntensityOfDirectLight = clamp( dot( -lightForwardDirection, normalVectorInWorldSpace.xyz ), 0.0, 1.0 ) * u_isDirectionalLight[i];
		lightTotalIntensity = lightIntensityFromCone * lightIntensityFromRadius * ( lightIntensityOfNonDirectLight + lightIntensityOfDirectLight ) * u_lightColorAndBrightness[i].a;
		lightTotalIntensity = SmoothStep(lightTotalIntensity);

		lightColor =  u_lightColorAndBrightness[i].rgb * lightTotalIntensity;
		totalDirectLightColor += lightColor;
		//------------------------------------------------------------------------------------------------------

		//Specular light----------------------------------------------------------------------------------------
		float specularIntensityOfNonDirectLight = clamp( dot( idealDirectionTolight, directionToPointLight ),0.0,1.0 ) * (1.0 - u_isDirectionalLight[i]);
		float specularIntensityOfDirectLight = clamp( dot( -lightForwardDirection , idealDirectionTolight ),0.0,1.0 ) * ( u_isDirectionalLight[i] );
		float specularIntensity = specularIntensityOfNonDirectLight + specularIntensityOfDirectLight;
		specularIntensity = pow ( specularIntensity, 1.0 + 16.0 * glossiness );
		specularIntensity *= ( lightIntensityFromCone * lightIntensityFromRadius * reflectivity * u_lightColorAndBrightness[i].a );
		totalSpecularColor += ( specularIntensity * u_lightColorAndBrightness[i].rgb );
		//------------------------------------------------------------------------------------------------------
	}
	vec4 surfaceColor = v_color;
	pixelFragmentColor = surfaceColor * diffuseTexel;

	pixelFragmentColor.rgb *= clamp( totalDirectLightColor * shadowFactor, vec3(0.0,0.0,0.0), vec3(1.0,1.0,1.0) );

	pixelFragmentColor.rgb += clamp( totalSpecularColor * shadowFactor, vec3(0.0,0.0,0.0), vec3(1.0,1.0,1.0) );

	//fog-------------------------------------------------------------------------------------------------------
	vec3 cameraToPixelDirection = normalize( cameraToPixel );
	float distancePixelToCamera = length(cameraToPixel);
	float fogIntensity = clamp( ( distancePixelToCamera - u_fogStartDistance ) / ( u_fogEndDistance - u_fogStartDistance ),0.0,1.0);
	fogIntensity *= u_fogColor.a;
	//----------------------------------------------------------------------------------------------------------

	vec4 finalColor = vec4( pixelFragmentColor.rgb * ( 1.0 - fogIntensity ) + ( u_fogColor.rgb * fogIntensity ), pixelFragmentColor.a );

	vec4 emissiveTexel = texture2D( u_emissiveTexture, texCoords );
	finalColor += emissiveTexel;

	o_fragColor = finalColor;
}