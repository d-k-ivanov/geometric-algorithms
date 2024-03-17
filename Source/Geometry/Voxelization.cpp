#include "Voxelization.h"

#include "Utils/ChronoUtilities.h"

#include <algorithm>
#include <map>
#include <set>

Voxelization::Voxelization()
    : tam()
    , xmax(0)
    , ymax(0)
    , zmax(0)
    , xmin(0)
    , ymin(0)
    , zmin(0)
    , numX(0)
    , numY(0)
    , numZ(0)
{
}

Voxelization::Voxelization(TriangleModel* modelo, glm::vec3 _tam, int tipo)
{
    tam              = _tam;
    AABB aabb_modelo = modelo->getAABB();
    xmax             = aabb_modelo.getMax().getX();
    ymax             = aabb_modelo.getMax().getY();
    zmax             = aabb_modelo.getMax().getZ();
    xmin             = aabb_modelo.getMin().getX();
    ymin             = aabb_modelo.getMin().getY();
    zmin             = aabb_modelo.getMin().getZ();
    numX             = ((xmax - xmin) / tam[0]) + 1;
    numY             = ((ymax - ymin) / tam[1]) + 1;
    numZ             = ((zmax - zmin) / tam[2]) + 1;

    double auxX, auxY, auxZ;
    auxX = xmin;
    auxY = ymin;
    auxZ = zmin;
    for(int i = 0; i < numX; i++)
    {
        std::vector<std::vector<Voxel*>> nivel1;
        for(int x = 0; x < numY; x++)
        {
            std::vector<Voxel*> nivel2;
            for(int y = 0; y < numZ; y++)
            {
                Vect3d puntoMinVoxel(auxX, auxY, auxZ);
                Voxel* insertado = new Voxel(puntoMinVoxel, tam);
                nivel2.push_back(insertado);
                auxZ += tam[2];
            }
            auxZ = zmin;
            auxY += tam[1];
            nivel1.push_back(nivel2);
        }
        auxY = ymin;
        auxX += tam[0];
        voxeles.push_back(nivel1);
    }
    int                     aux1       = modelo->getFaces().size();
    std::vector<Triangle3d> triangulos = modelo->getFaces();
    switch(tipo)
    {
        case 0:
            for(int i = 0; i < numX; i++)
            {
                for(int x = 0; x < numY; x++)
                {
                    for(int y = 0; y < numZ; y++)
                    {
                        for(int j = 0; j < aux1; j++)
                        {
                            if(voxeles[i][x][y]->fuerzaBruta(triangulos[j]))
                            {
                                voxeles[i][x][y]->setFormato(Formato_Voxel::GRIS);
                                j = aux1;
                            }
                        }
                    }
                }
            }
            break;
        case 1:
            lineaBarrido(modelo->getFaces());
            break;
        case 2:

            for(int j = 0; j < aux1; j++)
            {
                AABB                aabb_ti               = triangulos[j].getAABB();
                std::vector<Voxel*> voxeles_seleccionados = obtenerVoxeles(aabb_ti);
                for(int i = 0; i < voxeles_seleccionados.size(); i++)
                {
                    if(voxeles_seleccionados[i]->fuerzaBruta(modelo->getFaces()[j]))
                    {
                        voxeles_seleccionados[i]->setFormato(Formato_Voxel::GRIS);
                    }
                }
            }
            break;
    }
    this->flood();
}

Voxelization::Voxelization(double _xmax, double _ymax, double _zmax, double _xmin, double _ymin, double _zmin, glm::vec3 _tam)
{
    tam  = _tam;
    xmax = _xmax;
    ymax = _ymax;
    zmax = _zmax;
    xmin = _xmin;
    ymin = _ymin;
    zmin = _zmin;
    numX = ((xmax - xmin) / tam[0]) + 1;
    numY = ((ymax - ymin) / tam[1]) + 1;
    numZ = ((zmax - zmin) / tam[2]) + 1;

    double auxX, auxY, auxZ;
    auxX = xmin;
    auxY = ymin;
    auxZ = zmin;
    for(int i = 0; i < numX; i++)
    {
        std::vector<std::vector<Voxel*>> nivel1;
        for(int x = 0; x < numY; x++)
        {
            std::vector<Voxel*> nivel2;
            for(int y = 0; y < numZ; y++)
            {
                Vect3d puntoMinVoxel(auxX, auxY, auxZ);
                Voxel* insertado = new Voxel(puntoMinVoxel, tam);
                nivel2.push_back(insertado);
                auxZ += tam[2];
            }
            auxZ = zmin;
            auxY += tam[1];
            nivel1.push_back(nivel2);
        }
        auxY = ymin;
        auxX += tam[0];
        voxeles.push_back(nivel1);
    }
}

Voxelization::Voxelization(const Voxelization& voxel)
{
}

Voxelization::~Voxelization()
{
}

Voxel* Voxelization::obtenerVoxel(double x, double y, double z)
{
    int    i   = int(glm::abs(x / tam[0])) % numX;
    int    j   = int(glm::abs(y / tam[1])) % numY;
    int    k   = int(glm::abs(z / tam[2])) % numZ;
    Voxel* res = voxeles[i][j][k];

    return res;
}

void Voxelization::insertar(Vect3d dato)
{
    double x = dato.getX();
    double y = dato.getY();
    double z = dato.getZ();

    Voxel* v = obtenerVoxel(x, y, z);
    v->inserta(dato);
}

bool Voxelization::comp(const std::pair<Vect3d, int>& v1, const std::pair<Vect3d, int>& v2)
{
    std::pair<Vect3d, int> aux  = v1;
    std::pair<Vect3d, int> aux2 = v2;

    return aux.first.getY() < aux2.first.getY();
}

void Voxelization::lineaBarrido(std::vector<Triangle3d> triangulos)
{

    std::vector<std::pair<Vect3d, int>> vertices;

    for(int i = 0; i < triangulos.size(); i++)
    {
        Triangle3d triangulo = triangulos[i];
        vertices.push_back(std::pair(triangulo.getA(), i));
        vertices.push_back(std::pair(triangulo.getB(), i));
        vertices.push_back(std::pair(triangulo.getC(), i));
    }

    std::sort(vertices.begin(), vertices.end(), &comp);

    std::map<int, int>           triangulos_linea;
    std::vector<Voxel*>          voxeles_linea;
    std::map<int, int>::iterator it;
    int                          indice = 0;
    for(int i = 0; i < numY; i++)
    {
        double nivelMaxY = this->voxeles[0][i][0]->getMax().getY();
        double nivelMinY = this->voxeles[0][i][0]->getMin().getY();
        for(int x = 0; x < numX; x++)
        {
            for(int y = 0; y < numZ; y++)
            {
                voxeles_linea.push_back(this->voxeles[x][i][y]);
            }
        }
        for(int v = 0; v < vertices.size(); v++)
        {
            if((vertices[v].first.getY() > nivelMinY || BasicGeometry::equal(vertices[v].first.getY(), nivelMinY))
               && (vertices[v].first.getY() < nivelMaxY || BasicGeometry::equal(vertices[v].first.getY(), nivelMaxY)))
            {
                it = triangulos_linea.find(vertices[v].second);
                if(it != triangulos_linea.end())
                {
                    it->second += 1;
                }
                else
                {
                    triangulos_linea.insert(std::pair(vertices[v].second, 0));
                }
            }
            else if(vertices[v].first.getY() > nivelMaxY)
            {
                break;
            }
        }
        for(int j = 0; j < voxeles_linea.size(); j++)
        {
            for(it = triangulos_linea.begin(); it != triangulos_linea.end(); ++it)
            {
                if(voxeles_linea[j]->fuerzaBruta(triangulos[it->first]))
                {
                    voxeles_linea[j]->setFormato(Formato_Voxel::GRIS);
                    break;
                }
            }
        }
        it = triangulos_linea.begin();
        while(it != triangulos_linea.end())
        {
            if(it->second == 2)
            {
                it = triangulos_linea.erase(it);
            }
            else
            {
                it++;
            }
        }
        voxeles_linea.clear();
    }
}

std::vector<Voxel*> Voxelization::obtenerVoxeles(AABB aabb_ti)
{

    std::vector<std::pair<Voxel*, glm::vec3>> solucion;

    Vect3d e1(aabb_ti.getMin());
    Vect3d e2(aabb_ti.getMax());
    Vect3d e3(aabb_ti.getMin().getX(), aabb_ti.getMin().getY(), aabb_ti.getMax().getZ());
    Vect3d e4(aabb_ti.getMax().getX(), aabb_ti.getMin().getY(), aabb_ti.getMax().getZ());
    Vect3d e5(aabb_ti.getMin().getX(), aabb_ti.getMax().getY(), aabb_ti.getMax().getZ());
    Vect3d e6(aabb_ti.getMax().getX(), aabb_ti.getMin().getY(), aabb_ti.getMin().getZ());
    Vect3d e7(aabb_ti.getMin().getX(), aabb_ti.getMax().getY(), aabb_ti.getMin().getZ());
    Vect3d e8(aabb_ti.getMax().getX(), aabb_ti.getMax().getY(), aabb_ti.getMin().getZ());

    for(int i = 0; i < numX; i++)
    {
        for(int x = 0; x < numY; x++)
        {
            for(int y = 0; y < numZ; y++)
            {

                if(voxeles[i][x][y]->getFormato() != Formato_Voxel::GRIS)
                {
                    if(insideVoxel(voxeles[i][x][y], e1) || insideVoxel(voxeles[i][x][y], e2) || insideVoxel(voxeles[i][x][y], e3) || insideVoxel(voxeles[i][x][y], e4)
                       || insideVoxel(voxeles[i][x][y], e5) || insideVoxel(voxeles[i][x][y], e6) || insideVoxel(voxeles[i][x][y], e7) || insideVoxel(voxeles[i][x][y], e8))
                    {
                        glm::vec3 aux(i, x, y);
                        solucion.push_back(std::pair(voxeles[i][x][y], aux));
                    }
                }
            }
        }
    }
    std::vector<Voxel*> res;
    for(int i = 0; i < solucion.size(); i++)
    {
        res.push_back(solucion[i].first);
    }

    return res;
}

bool Voxelization::insideVoxel(Voxel* voxel, Vect3d v)
{
    return v.getX() >= voxel->getMin().getX() && v.getX() <= voxel->getMax().getX() && v.getY() >= voxel->getMin().getY() && v.getY() <= voxel->getMax().getY() && v.getZ() >= voxel->getMin().getZ() && v.getZ() <= voxel->getMax().getZ();
}

void Voxelization::flood()
{
    int centralX, centralY, centralZ;
    centralX = numX / 2;
    centralY = numY / 2;
    centralZ = numZ / 2;

    if(this->voxeles[centralX][centralY][centralZ]->getFormato() == Formato_Voxel::GRIS)
    {
        while(this->voxeles[centralX][centralY][centralZ]->getFormato() == Formato_Voxel::GRIS)
        {
            centralX++;
        }
    }
    recursivo(this->voxeles[centralX][centralY][centralZ], centralX, centralY, centralZ);
    for(int i = 0; i < numX; i++)
    {
        for(int x = 0; x < numY; x++)
        {
            for(int y = 0; y < numZ; y++)
            {
                if(this->voxeles[i][x][y]->getFormato() == Formato_Voxel::NP)
                {
                    this->voxeles[i][x][y]->setFormato(Formato_Voxel::BLANCO);
                }
            }
        }
    }
}

void Voxelization::recursivo(Voxel* v, int x, int y, int z)
{
    if(v->getFormato() == Formato_Voxel::GRIS || v->getFormato() == Formato_Voxel::NEGRO)
    {
        return;
    }
    v->setFormato(Formato_Voxel::NEGRO);

    if(x + 1 < numX)
    {
        recursivo(this->voxeles[x + 1][y][z], x + 1, y, z);
    }
    if(x - 1 > 0)
    {
        recursivo(this->voxeles[x - 1][y][z], x - 1, y, z);
    }
    if(y + 1 < numY)
    {
        recursivo(this->voxeles[x][y + 1][z], x, y + 1, z);
    }
    if(y - 1 > 0)
    {
        recursivo(this->voxeles[x][y - 1][z], x, y - 1, z);
    }
    if(z + 1 < numZ)
    {
        recursivo(this->voxeles[x][y][z + 1], x, y, z + 1);
    }
    if(z - 1 > 0)
    {
        recursivo(this->voxeles[x][y][z - 1], x, y, z - 1);
    }
}

AlgGeom::DrawVoxelization* Voxelization::getRenderingObject(bool gris)
{
    Formato_Voxel formatin;
    if(gris)
        formatin = Formato_Voxel::GRIS;
    else
        formatin = Formato_Voxel::NEGRO;

    std::vector<glm::vec3> vector;
    Vect3d            despl(tam[0] / 2, tam[1] / 2, tam[2] / 2);
    for(int i = 0; i < numX; i++)
    {
        for(int x = 0; x < numY; x++)
        {
            for(int y = 0; y < numZ; y++)
            {
                if(this->getVoxeles()[i][x][y]->getFormato() == formatin)
                {
                    Vect3d centro = this->getVoxeles()[i][x][y]->getMin().add(despl);
                    vector.push_back(glm::vec3(centro.getX(), centro.getY(), centro.getZ()));
                }
            }
        }
    }
    glm::vec3* positions = new glm::vec3[vector.size()];
    glm::vec3* color     = new glm::vec3[vector.size()];
    for(int i = 0; i < vector.size(); i++)
    {
        positions[i] = vector[i];
        color[i]     = glm::vec3(0, 0, 0);
    }

    AlgGeom::DrawVoxelization* voxelization = new AlgGeom::DrawVoxelization(positions, vector.size(), tam, color);

    // delete positions;
    // delete color;
    return voxelization;
}

bool Voxelization::rayTraversal(Ray3d& r, std::vector<Voxel*>& v)
{
    double      tMin;
    double      tMax;
    const bool ray_intersects_grid = rayBoxIntersection(r, tMin, tMax, 0, 1);
    if(!ray_intersects_grid)
        return 0;

    tMin             = BasicGeometry::max2(tMin, 0);
    tMax             = BasicGeometry::max2(tMax, 1);
    Vect3d aux       = r.getOrigin();
    Vect3d ray_start = r.getDirection().scalarMul(tMin).add(aux);
    Vect3d ray_end   = r.getDirection().scalarMul(tMax).add(aux);

    size_t       current_X_index = BasicGeometry::max2(1, std::ceil((ray_start.getX() - this->getXMin()) / this->tam.x));
    const size_t end_X_index     = BasicGeometry::max2(1, std::ceil((ray_end.getX() - this->getXMin()) / this->tam.x));
    int          stepX;
    double        tDeltaX;
    double        tMaxX;
    if(r.getDirection().getX() > 0.0)
    {
        stepX   = 1;
        tDeltaX = this->tam[0] / r.getDirection().getX();
        tMaxX   = tMin + (this->getXMin() + current_X_index * this->tam.x - ray_start.getX()) / r.getDirection().getX();
    }
    else if(r.getDirection().getX() < 0.0)
    {
        stepX                         = -1;
        tDeltaX                       = this->tam[0] / -r.getDirection().getX();
        const size_t previous_X_index = current_X_index - 1;
        tMaxX                         = tMin + (this->getXMin() + previous_X_index * this->tam.x - ray_start.getX()) / r.getDirection().getX();
    }
    else
    {
        stepX   = 0;
        tDeltaX = tMax;
        tMaxX   = tMax;
    }

    size_t       current_Y_index = BasicGeometry::max2(1, std::ceil((ray_start.getY() - this->getYMin()) / this->tam.y));
    const size_t end_Y_index     = BasicGeometry::max2(1, std::ceil((ray_end.getY() - this->getYMin()) / this->tam.y));
    int          stepY;
    double        tDeltaY;
    double        tMaxY;
    if(r.getDirection().getY() > 0.0)
    {
        stepY   = 1;
        tDeltaY = this->tam[1] / r.getDirection().getY();
        tMaxY   = tMin + (this->getYMin() + current_Y_index * this->tam.y - ray_start.getY()) / r.getDirection().getY();
    }
    else if(r.getDirection().getY() < 0.0)
    {
        stepY                         = -1;
        tDeltaY                       = this->tam[1] / -r.getDirection().getY();
        const size_t previous_Y_index = current_Y_index - 1;
        tMaxY                         = tMin + (this->getYMin() + previous_Y_index * this->tam.y - ray_start.getY()) / r.getDirection().getY();
    }
    else
    {
        stepY   = 0;
        tDeltaY = tMax;
        tMaxY   = tMax;
    }

    size_t       current_Z_index = BasicGeometry::max2(1, std::ceil((ray_start.getZ() - this->getZMin()) / this->tam.z));
    const size_t end_Z_index     = BasicGeometry::max2(1, std::ceil((ray_end.getZ() - this->getZMin()) / this->tam.z));
    int          stepZ;
    double        tDeltaZ;
    double        tMaxZ;
    if(r.getDirection().getZ() > 0.0)
    {
        stepZ   = 1;
        tDeltaZ = this->tam[2] / r.getDirection().getZ();
        tMaxZ   = tMin + (this->getZMin() + current_Z_index * this->tam.z - ray_start.getZ()) / r.getDirection().getZ();
    }
    else if(r.getDirection().getZ() < 0.0)
    {
        stepZ                         = -1;
        tDeltaZ                       = this->tam[2] / -r.getDirection().getZ();
        const size_t previous_Z_index = current_Z_index - 1;
        tMaxZ                         = tMin + (this->getZMin() + previous_Z_index * this->tam.z - ray_start.getZ()) / r.getDirection().getZ();
    }
    else
    {
        stepZ   = 0;
        tDeltaZ = tMax;
        tMaxZ   = tMax;
    }
    while(glm::all(glm::greaterThanEqual(glm::vec3(current_X_index, current_Y_index, current_Z_index), glm::vec3(1, 1, 1))) && glm::all(glm::lessThanEqual(glm::vec3(current_X_index, current_Y_index, current_Z_index), glm::vec3(numX, numY, numZ))))
    {
        /*while (current_X_index != end_X_index && current_Y_index != end_Y_index && current_Z_index != end_Z_index) {*/
        v.push_back(this->voxeles[current_X_index - 1][current_Y_index - 1][current_Z_index - 1]);
        if(tMaxX < tMaxY && tMaxX < tMaxZ)
        {
            // X-axis traversal.
            current_X_index += stepX;
            tMaxX += tDeltaX;
        }
        else if(tMaxY < tMaxZ)
        {
            // Y-axis traversal.
            current_Y_index += stepY;
            tMaxY += tDeltaY;
        }
        else
        {
            // Z-axis traversal.
            current_Z_index += stepZ;
            tMaxZ += tDeltaZ;
        }
    }
    if(v.size() == 0)
        return false;
    return true;
}

bool Voxelization::rayBoxIntersection(Ray3d& r, double& tMin, double& tMax, double t0, double t1)
{
    double       tYMin, tYMax, tZMin, tZMax;
    const double x_inv_dir = 1 / r.getDirection().getX();
    if(x_inv_dir >= 0)
    {
        tMin = (this->getXMin() - r.getOrigin().getX()) * x_inv_dir;
        tMax = (this->getXMax() - r.getOrigin().getX()) * x_inv_dir;
    }
    else
    {
        tMin = (this->getXMax() - r.getOrigin().getX()) * x_inv_dir;
        tMax = (this->getXMin() - r.getOrigin().getX()) * x_inv_dir;
    }

    const double y_inv_dir = 1 / r.getDirection().getY();
    if(y_inv_dir >= 0)
    {
        tYMin = (this->getYMin() - r.getOrigin().getY()) * y_inv_dir;
        tYMax = (this->getYMax() - r.getOrigin().getY()) * y_inv_dir;
    }
    else
    {
        tYMin = (this->getYMax() - r.getOrigin().getY()) * y_inv_dir;
        tYMax = (this->getYMin() - r.getOrigin().getY()) * y_inv_dir;
    }

    if(tMin > tYMax || tYMin > tMax)
        return false;
    if(tYMin > tMin)
        tMin = tYMin;
    if(tYMax < tMax)
        tMax = tYMax;

    const double z_inv_dir = 1 / r.getDirection().getZ();
    if(z_inv_dir >= 0)
    {
        tZMin = (this->getZMin() - r.getOrigin().getZ()) * z_inv_dir;
        tZMax = (this->getZMax() - r.getOrigin().getZ()) * z_inv_dir;
    }
    else
    {
        tZMin = (this->getZMax() - r.getOrigin().getZ()) * z_inv_dir;
        tZMax = (this->getZMin() - r.getOrigin().getZ()) * z_inv_dir;
    }

    if(tMin > tZMax || tZMin > tMax)
        return false;
    if(tZMin > tMin)
        tMin = tZMin;
    if(tZMax < tMax)
        tMax = tZMax;
    return (tMin < t1 && tMax > t0);
}

Voxelization* Voxelization::AND(Voxelization& vox)
{

    Voxelization* res;
    glm::vec3     num_voxeles;

    glm::vec3 tamNuevo(BasicGeometry::max2(this->getXMax() - this->getXMin(), vox.getXMax() - vox.getXMin()), BasicGeometry::max2(this->getYMax() - this->getYMin(), vox.getYMax() - vox.getYMin()), BasicGeometry::max2(this->getZMax() - this->getZMin(), vox.getZMax() - vox.getZMin()));

    res = new Voxelization(BasicGeometry::max2(this->getXMax(), vox.getXMax()), BasicGeometry::max2(this->getYMax(), vox.getYMax()),
                           BasicGeometry::max2(this->getZMax(), vox.getZMax()), BasicGeometry::min2(this->getXMin(), vox.getXMin()),
                           BasicGeometry::min2(this->getYMin(), vox.getYMin()), BasicGeometry::min2(this->getZMin(), vox.getZMin()), glm::vec3(tam.x));

    num_voxeles = glm::vec3(res->numX, res->numY, res->numZ);
    int xV, yV, zV;
    int x2, y2, z2;
    for(int i = 0; i < num_voxeles.x; i++)
    {
        for(int y = 0; y < num_voxeles.y; y++)
        {
            for(int z = 0; z < num_voxeles.z; z++)
            {
                if(comprobarPertenencia(res->getVoxeles()[i][y][z], this, xV, yV, zV))
                {
                    if(comprobarPertenencia(res->getVoxeles()[i][y][z], &vox, x2, y2, z2))
                    {
                        if(this->getVoxeles()[xV][yV][zV]->getFormato() != Formato_Voxel::BLANCO && vox.getVoxeles()[x2][y2][z2]->getFormato() != Formato_Voxel::BLANCO)
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::NEGRO);
                        else
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                    else
                    {
                        res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                }
                else
                {
                    res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                }
            }
        }
    }

    return res;
}

Voxelization* Voxelization::OR(Voxelization& vox)
{

    Voxelization* res;
    glm::vec3     num_voxeles;

    glm::vec3 tamNuevo(BasicGeometry::max2(this->getXMax() - this->getXMin(), vox.getXMax() - vox.getXMin()), BasicGeometry::max2(this->getYMax() - this->getYMin(), vox.getYMax() - vox.getYMin()), BasicGeometry::max2(this->getZMax() - this->getZMin(), vox.getZMax() - vox.getZMin()));

    res = new Voxelization(BasicGeometry::max2(this->getXMax(), vox.getXMax()), BasicGeometry::max2(this->getYMax(), vox.getYMax()),
                           BasicGeometry::max2(this->getZMax(), vox.getZMax()), BasicGeometry::min2(this->getXMin(), vox.getXMin()),
                           BasicGeometry::min2(this->getYMin(), vox.getYMin()), BasicGeometry::min2(this->getZMin(), vox.getZMin()), glm::vec3(tam.x));

    num_voxeles = glm::vec3(res->numX, res->numY, res->numZ);

    int xV, yV, zV;
    int x2, y2, z2;
    for(int i = 0; i < num_voxeles.x; i++)
    {
        for(int y = 0; y < num_voxeles.y; y++)
        {
            for(int z = 0; z < num_voxeles.z; z++)
            {
                if(comprobarPertenencia(res->getVoxeles()[i][y][z], this, xV, yV, zV))
                {
                    if(comprobarPertenencia(res->getVoxeles()[i][y][z], &vox, x2, y2, z2))
                    {
                        if(this->getVoxeles()[xV][yV][zV]->getFormato() != Formato_Voxel::BLANCO || vox.getVoxeles()[x2][y2][z2]->getFormato() != Formato_Voxel::BLANCO)
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::NEGRO);
                        else
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                    else
                    {
                        if(this->getVoxeles()[xV][yV][zV]->getFormato() != Formato_Voxel::BLANCO)
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::NEGRO);
                        else
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                }
                else
                {
                    if(comprobarPertenencia(res->getVoxeles()[i][y][z], &vox, x2, y2, z2))
                    {
                        if(vox.getVoxeles()[x2][y2][z2]->getFormato() != Formato_Voxel::BLANCO)
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::NEGRO);
                        else
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                    else
                    {
                        res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                }
            }
        }
    }

    return res;
}

Voxelization* Voxelization::XOR(Voxelization& vox)
{

    Voxelization* res;
    glm::vec3     num_voxeles;

    glm::vec3 tamNuevo(BasicGeometry::max2(this->getXMax() - this->getXMin(), vox.getXMax() - vox.getXMin()), BasicGeometry::max2(this->getYMax() - this->getYMin(), vox.getYMax() - vox.getYMin()), BasicGeometry::max2(this->getZMax() - this->getZMin(), vox.getZMax() - vox.getZMin()));

    res = new Voxelization(BasicGeometry::max2(this->getXMax(), vox.getXMax()), BasicGeometry::max2(this->getYMax(), vox.getYMax()),
                           BasicGeometry::max2(this->getZMax(), vox.getZMax()), BasicGeometry::min2(this->getXMin(), vox.getXMin()),
                           BasicGeometry::min2(this->getYMin(), vox.getYMin()), BasicGeometry::min2(this->getZMin(), vox.getZMin()), glm::vec3(tam.x));

    num_voxeles = glm::vec3(res->numX, res->numY, res->numZ);

    int xV, yV, zV;
    int x2, y2, z2;
    for(int i = 0; i < num_voxeles.x; i++)
    {
        for(int y = 0; y < num_voxeles.y; y++)
        {
            for(int z = 0; z < num_voxeles.z; z++)
            {
                if(comprobarPertenencia(res->getVoxeles()[i][y][z], this, xV, yV, zV))
                {
                    if(comprobarPertenencia(res->getVoxeles()[i][y][z], &vox, x2, y2, z2))
                    {
                        if(this->getVoxeles()[xV][yV][zV]->getFormato() != Formato_Voxel::BLANCO && vox.getVoxeles()[x2][y2][z2]->getFormato() == Formato_Voxel::BLANCO)
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::NEGRO);
                        else
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                    else
                    {
                        if(this->getVoxeles()[xV][yV][zV]->getFormato() != Formato_Voxel::BLANCO)
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::NEGRO);
                        else
                            res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                    }
                }
                else
                {
                    res->getVoxeles()[i][y][z]->setFormato(Formato_Voxel::BLANCO);
                }
            }
        }
    }

    return res;
}

bool Voxelization::comprobarPertenencia(Voxel* v, Voxelization* voxelizacion, int& x, int& y, int& z)
{
    if((BasicGeometry::equal(v->getMin().getX(), voxelizacion->getXMin()) || v->getMin().getX() > voxelizacion->getXMin())
       && (BasicGeometry::equal(v->getMax().getX(), voxelizacion->getXMax()) || v->getMax().getX() < voxelizacion->getXMax()))
    {

        if((BasicGeometry::equal(v->getMin().getY(), voxelizacion->getYMin()) || v->getMin().getY() > voxelizacion->getYMin())
           && (BasicGeometry::equal(v->getMax().getY(), voxelizacion->getYMax()) || v->getMax().getY() < voxelizacion->getYMax()))
        {

            if((BasicGeometry::equal(v->getMin().getZ(), voxelizacion->getZMin()) || v->getMin().getZ() > voxelizacion->getZMin())
               && (BasicGeometry::equal(v->getMax().getZ(), voxelizacion->getZMax()) || v->getMax().getZ() < voxelizacion->getZMax()))
            {

                x = (v->getMin().getX() - voxelizacion->getXMin()) / voxelizacion->getTam().x;
                y = (v->getMin().getY() - voxelizacion->getYMin()) / voxelizacion->getTam().y;
                z = (v->getMin().getZ() - voxelizacion->getZMin()) / voxelizacion->getTam().z;

                return true;
            }
        }
    }
    return false;
}