#version 330 core
// Hash functions by Nimitz:
// https://www.shadertoy.com/view/Xt3cDn
//

float g_seed = 0.0;
uint base_hash(uvec2 p) {
    p = 1103515245U*((p >> 1U)^(p.yx));
    uint h32 = 1103515245U*((p.x)^(p.y>>3U));
    return h32^(h32 >> 16);
}

float hash1(inout float seed) {
    uint n = base_hash(floatBitsToUint(vec2(seed+=.1,seed+=.1)));
    return float(n)/float(0xffffffffU);
}

vec2 hash2(inout float seed) {
    uint n = base_hash(floatBitsToUint(vec2(seed+=.1,seed+=.1)));
    uvec2 rz = uvec2(n, n*48271U);
    return vec2(rz.xy & uvec2(0x7fffffffU))/float(0x7fffffff);
}

vec3 hash3(inout float seed) {
    uint n = base_hash(floatBitsToUint(vec2(seed+=.1,seed+=.1)));
    uvec3 rz = uvec3(n, n*16807U, n*48271U);
    return vec3(rz & uvec3(0x7fffffffU))/float(0x7fffffff);
}

//
// Ray trace helper functions
//

vec3 random_cos_weighted_hemisphere_direction( const vec3 n, inout float seed ) {
  	vec2 r = hash2(seed);
	vec3  uu = normalize(cross(n, abs(n.y) > .5 ? vec3(1.,0.,0.) : vec3(0.,1.,0.)));
	vec3  vv = cross(uu, n);
	float ra = sqrt(r.y);
	float rx = ra*cos(6.28318530718*r.x); 
	float ry = ra*sin(6.28318530718*r.x);
	float rz = sqrt(1.-r.y);
	vec3  rr = vec3(rx*uu + ry*vv + rz*n);
    return normalize(rr);
}
#define Samples 16
#define MaxDepth 10
#define T_MIN 1e-3
#define T_MAX 1000.0
#define CornellBoxSize 2.0

#define LAMBERTIAN 0
#define METAL 1
#define DIELETRIC 2
#define EMISSIVE 3
struct Material{
	int matType;
    vec3 albedo;
    float ior;
};
struct Sphere{
	vec3 center;
    float radius;
    Material material;
};
 struct Plane{
	vec3 p;
    vec3 normal;
    Material material;
};
struct Rectangle{
	Plane plane;
    float size;
};
Sphere sceneList[] =Sphere[](Sphere(vec3(1.0,-1.5,-1.0),0.5,Material(METAL,vec3(0.8,0.8,0.8),0.0)),
    Sphere(vec3(-0.0,-1.5,-0.5),0.5,Material(DIELETRIC,vec3(0.8,0.8,0.8),1.3)),
    Sphere(vec3(-0.5,-1.5,-1.5),0.5,Material(LAMBERTIAN,vec3(0.1,1.0,0.1),0.0)));

Rectangle planeList[]=Rectangle[](
    //Left wall
    Rectangle(Plane(vec3(-CornellBoxSize,0.0,0.0),vec3(1.0,0.0,0.0),Material(LAMBERTIAN,vec3(0.1,1.0,0.1),0.0)),CornellBoxSize),
    //Right wall
     Rectangle(Plane(vec3(CornellBoxSize,0.0,0.0),vec3(1.0,0.0,0.0),Material(LAMBERTIAN,vec3(1.0,0.1,0.1),0.0)),CornellBoxSize),
    //back
     Rectangle(Plane(vec3(0.0,0.0,-CornellBoxSize),vec3(0.0,0.0,1.0),Material(LAMBERTIAN,vec3(0.5,0.5,0.5),0.0)),CornellBoxSize),
    //floor
     Rectangle(Plane(vec3(0.0,-CornellBoxSize,0.0),vec3(0.0,1.0,0.0),Material(LAMBERTIAN,vec3(0.5,0.5,0.5),0.0)),CornellBoxSize),
    //ceiling
     Rectangle(Plane(vec3(0.0,CornellBoxSize,0.0),vec3(0.0,1.0,0.0),Material(LAMBERTIAN,vec3(0.5,0.5,0.5),0.0)),CornellBoxSize)
);

Rectangle lightList[]=Rectangle[](
Rectangle(Plane(vec3(0.0,2.0,0.0),vec3(0.0,1.0,0.0),Material(EMISSIVE,vec3(1.0,1.0,1.0),0.0)),1.0)
);
struct HitRecord{
    bool isHit;
    vec3 p;
    vec3 normal;
    Material material;
    
};
struct Ray{
    vec3 origin;
    vec3 direction;
    };
//Primitive:Sphere
float intersectSphere(Ray r,Sphere s){
    vec3 oc=r.origin-s.center;
    vec3 t=r.direction;
	float a=dot(t,t);
    float b=2.0*dot(t,oc);
    float c=dot(oc,oc)-s.radius*s.radius;
    float discriminant=b*b-4.0*a*c;
    if (discriminant<=0.0)
        return -1.0;
    else
    {
    	float x1=(-b-sqrt(discriminant))/(2.0*a);
        if(x1>T_MIN&&x1<T_MAX)
        	return x1;
        x1=(-b+sqrt(discriminant))/(2.0*a);
         if(x1>T_MIN&&x1<T_MAX)
        	return x1;
    }
}
vec3 getSphereNormal(vec3 hitPoint,Sphere s){
	return normalize(hitPoint-s.center);
}
//Primitive:Plane
float intersectPlane(Ray r,Plane s){
	vec3 op=s.p-r.origin;
    float d=dot(op,s.normal)/dot(r.direction,s.normal);
    return d;

}
vec3 getPlaneNormal(vec3 hitPoint,Plane p){
	return normalize(hitPoint+p.normal);
}
//Primitive:Rectangle
float intersectRectangle(Ray r, Rectangle rec){
    float d=intersectPlane(r,rec.plane);
    vec3 hitPoint=r.origin+d*r.direction;
    float size=rec.size;
    //XZ_PLANE
    if (rec.plane.normal==vec3(0.0,1.0,0.0)){
    	vec3 tmin=rec.plane.p-vec3(size,0.0,size);
        vec3 tmax=rec.plane.p+vec3(size,0.0,size);
        if (hitPoint.x>tmin.x&&hitPoint.x<tmax.x)
            if (hitPoint.z>tmin.z&&hitPoint.z<tmax.z)
                return d;
     //YZ_PLANE
    }else if (rec.plane.normal==vec3(1.0,0.0,0.0)){
    	vec3 tmin=rec.plane.p-vec3(0.0,size,size);
        vec3 tmax=rec.plane.p+vec3(0.0,size,size);
        if (hitPoint.y>tmin.y&&hitPoint.y<tmax.y)
            if (hitPoint.z>tmin.z&&hitPoint.z<tmax.z)
                return d;
    }//XY_PLANE
    else if (rec.plane.normal==vec3(0.0,0.0,1.0)){
    	vec3 tmin=rec.plane.p-vec3(size,size,0.0);
        vec3 tmax=rec.plane.p+vec3(size,size,0.0);
        if (hitPoint.x>tmin.x&&hitPoint.x<tmax.x)
            if (hitPoint.y>tmin.y&&hitPoint.y<tmax.y)
                return d;
    }
    
    return -1.0;
    
}
vec3 getRectangleNormal(Ray r,Rectangle rec){
    vec3 normal=normalize(rec.plane.normal);
    if (dot(r.direction,rec.plane.normal)>T_MIN)
    {
    	return  -normal;
    }else
		return normal;
}

vec3 skyColor(float t){
    return (1.0-t)*vec3(1.0)+t*vec3(0.2,0.5,0.7);
	
}

vec3 checkerTexture(vec2 uv, vec3 dir){
	float sines=sin(dir.x*10.0)*sin(dir.y*10.0)*sin(dir.z*10.0);
    if (sines>0.0)
        return vec3(1.0);
    else
        return vec3(0.0);
}

HitRecord intersectScene(Ray r){
    float tMax=T_MAX;
    float tMin=T_MIN;
    HitRecord hit_record;
    hit_record.isHit=false;
    //interseciton of the spheres 
    for (int i=0;i<sceneList.length();i++)
    {
    	float ts=intersectSphere(r, sceneList[i]);
   		 if (ts>tMin && ts<tMax){
            tMax=ts;
             hit_record.p=r.origin+r.direction*ts;
            hit_record.normal=getSphereNormal(hit_record.p,sceneList[i]);
             hit_record.material=sceneList[i].material;
             hit_record.isHit=true;
         }
        
    }
    //intersection of the Light emitter
    for (int i=0;i<lightList.length();i++)
    {
    	 float tp=intersectRectangle(r,lightList[i]);
   		 if (tp>tMin && tp<tMax){
            tMax=tp;
             hit_record.p=r.origin+r.direction*tp;
            hit_record.normal=getPlaneNormal(hit_record.p,lightList[i].plane);
             hit_record.material=lightList[i].plane.material;
             hit_record.isHit=true;
         }
        
    }
   
    //intersection of the cornell box
    for (int i=0;i<planeList.length();i++)
    {
    	 float tp=intersectRectangle(r,planeList[i]);
   		 if (tp>tMin && tp<tMax){
            tMax=tp;
             hit_record.p=r.origin+r.direction*tp;
            hit_record.normal=getRectangleNormal(r,planeList[i]);
             hit_record.material=planeList[i].plane.material;
             hit_record.isHit=true;
         }
        
    }
     
    return hit_record;

}
float schlick(float cos_theta, float n2)
{
    const float n1 = 1.0f;  // refraction index for air

    float r0s = (n1 - n2) / (n1 + n2);
    float r0 = r0s * r0s;

    return r0 + (1.0f - r0) * pow((1.0f - cos_theta), 5.0f);
}
bool refractVec(vec3 v, vec3 n, float ni_over_nt, out vec3 refracted)
{
    vec3 uv = normalize(v);

    float dt = dot(uv, n);

    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0f - dt * dt);

    if (discriminant > 0.0f)
    {
        refracted = ni_over_nt*(uv - n * dt) - n * sqrt(discriminant);

        return true;
    }
    else
        return false;
}
bool material_bsdf(HitRecord hit_record,Ray inRay, out Ray outRay,out vec3 attenuation){
    vec3 col=vec3(0.0);
    //lambertian
    if(hit_record.material.matType==LAMBERTIAN){
        vec3 ray_origin=hit_record.p;
        vec3 ray_dir=random_cos_weighted_hemisphere_direction(hit_record.normal,g_seed);
        outRay=Ray(ray_origin,ray_dir);
        attenuation=hit_record.material.albedo;
        return true;
    }else if(hit_record.material.matType==METAL){
     //metal
        vec3 reflected=reflect(inRay.direction,hit_record.normal);
        outRay=Ray(hit_record.p,reflected);
        attenuation=hit_record.material.albedo;
         return true;
    }else if(hit_record.material.matType==DIELETRIC){
     //dieletric
        vec3 outward_normal,refracted;
        float ni_over_nt,cosine,reflect_prob;
        vec3 reflected=reflect(inRay.direction,hit_record.normal);
        if (dot(inRay.direction,hit_record.normal)>0.0){
        	outward_normal=-hit_record.normal;
            ni_over_nt=hit_record.material.ior;
            cosine=dot(inRay.direction,hit_record.normal)/length(inRay.direction);
            cosine=sqrt(1.0-ni_over_nt*ni_over_nt*(1.0-cosine*cosine));
            
        }else{
            outward_normal=hit_record.normal;
        	ni_over_nt = 1.0f / hit_record.material.ior;
            cosine=-dot(inRay.direction,hit_record.normal)/length(inRay.direction);
        }
        if (refractVec(inRay.direction, outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cosine, hit_record.material.ior);
        else
            reflect_prob = 1.0f;
  
        if (hash1(g_seed)<reflect_prob)
        {
        	outRay=Ray(hit_record.p,reflected);

        } 	
        else
        {

            outRay=Ray(hit_record.p,refracted);
        }
        attenuation=hit_record.material.albedo;
        return true;

        
    }
    return false;

}

vec3 radiance(Ray r,vec2 uv,Rectangle rec){
  	vec3 col=vec3(1.0);
    HitRecord hit_record;
    for(int bounce=0;bounce<MaxDepth;bounce++)
    {
        hit_record=intersectScene(r);
        vec3 emitter=rec.plane.material.albedo;
        if (hit_record.isHit)
        {
			
            Ray outRay;
            vec3 color;

            bool scattered=material_bsdf(hit_record,r,outRay,color);
            r.origin=outRay.origin;
            r.direction=outRay.direction;
            if (scattered)
            {
                col*=color;
            }
            
            else
            {
            	col*=emitter;
                break;
            }
               
  
        }else
        {
            col*=vec3(0.0);
        	break;
        }
        
    }
    return col;
}


out vec4 fragColor;
vec2 resolution=vec2(800.0,600.0);
void main()
{
     // Normalized pixel coordinates (from 0 to 1)
    vec2 uv=gl_FragCoord.xy/resolution;
    g_seed = float(base_hash(floatBitsToUint(gl_FragCoord.xy)))/float(0xffffffffU);
    //camera ray calculation
    vec3 cameraPos=vec3(0.0,0.0,3.5);
    vec3 cameraLookAt=vec3(0.0,0.0,-1.0);
    float zNear=1.0,zFar=1000.0;
    float fov=60.0;
    float height=zNear*tan(radians(fov/2.0));
    float ratio=resolution.x/resolution.y;
    float viewport_height=2.0*height;
    float viewport_width=viewport_height*ratio;

    vec3 t=normalize(cameraLookAt-cameraPos);
    vec3 up=vec3(0.0,1.0,0.0);
    vec3 h=normalize(cross(t,up));
    vec3 v=normalize(cross(h,t));
    vec3 horizontal=viewport_width*h;
    vec3 vertical=viewport_height*v;

    vec3 leftBotCorner=cameraPos+t-horizontal*0.5-vertical*0.5;
    vec3 rayDir=leftBotCorner+uv.x*horizontal+uv.y*vertical-cameraPos;
    Ray r=Ray(cameraPos,rayDir);
 
     vec3 col=vec3(0.0);

     for(int i=0;i<Samples;i++)
     {
         float x=(gl_FragCoord.x+ hash1(g_seed))/resolution.x;
         float y=(gl_FragCoord.y+ hash1(g_seed))/resolution.y;
         for(int n=0;n<lightList.length();n++)
            col+=radiance(r,vec2(x,y),lightList[n]);
     }
    col/=float(Samples);
    // Output to screen
    fragColor =vec4(pow(col.x,1.0/2.2),pow(col.y,1.0/2.2),pow(col.z,1.0/2.2),1.0);
}