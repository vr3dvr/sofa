/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
#ifndef SOFA_COMPONENT_FORCEFIELD_TRIANGULARQUADRATICSPRINGSFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_TRIANGULARQUADRATICSPRINGSFORCEFIELD_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/componentmodel/behavior/ForceField.h>
#include <sofa/component/MechanicalObject.h>
#include <sofa/core/VisualModel.h>
#include <sofa/component/topology/MeshTopology.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/component/topology/TriangleData.h>
#include <sofa/component/topology/EdgeData.h>


namespace sofa
{

namespace component
{


namespace forcefield
{

using namespace sofa::defaulttype;
using namespace sofa::component::topology;


template<class DataTypes>
class TriangularQuadraticSpringsForceField : public core::componentmodel::behavior::ForceField<DataTypes>, public core::VisualModel
{
public:
    typedef core::componentmodel::behavior::ForceField<DataTypes> Inherited;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord    Coord   ;
    typedef typename DataTypes::Deriv    Deriv   ;
    typedef typename Coord::value_type   Real    ;


    class Mat3 : public fixed_array<Deriv,3>
    {
    public:
        Deriv operator*(const Deriv& v)
        {
            return Deriv((*this)[0]*v,(*this)[1]*v,(*this)[2]*v);
        }
        Deriv transposeMultiply(const Deriv& v)
        {
            return Deriv(v[0]*((*this)[0])[0]+v[1]*((*this)[1])[0]+v[2]*((*this)[2][0]),
                    v[0]*((*this)[0][1])+v[1]*((*this)[1][1])+v[2]*((*this)[2][1]),
                    v[0]*((*this)[0][2])+v[1]*((*this)[1][2])+v[2]*((*this)[2][2]));
        }
    };

protected:


    class EdgeRestInformation
    {
    public:
        Real  restLength;	// the rest length
        Real  currentLength; 	// the current edge length
        Real  dl;  // the current unit direction
        Real stiffness;

        EdgeRestInformation()
        {
        }
    };

    class TriangleRestInformation
    {
    public:
        Real  gamma[3];	// the angular stiffness
        Real stiffness[3]; // the elongation stiffness
        Mat3 DfDx[3]; /// the edge stiffness matrix

        TriangleRestInformation()
        {
        }
    };

    TriangleData<TriangleRestInformation> triangleInfo;
    EdgeData<EdgeRestInformation> edgeInfo;

    TriangleSetTopology<DataTypes> * _mesh;
    Data< VecCoord > _initialPoints;										///< the intial positions of the points

    bool updateMatrix;

    Data<Real> f_poissonRatio;
    Data<Real> f_youngModulus;
    Data<Real> f_dampingRatio;
    Data<bool> f_useAngularSprings; // whether angular springs should be included

    Real lambda;  /// first Lam� coefficient
    Real mu;    /// second Lam� coefficient
public:

    TriangularQuadraticSpringsForceField();

    TriangleSetTopology<DataTypes> *getTriangularTopology() const {return _mesh;}

    virtual ~TriangularQuadraticSpringsForceField();

    virtual double getPotentialEnergy(const VecCoord& x);

    virtual void init();
    virtual void addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v);
    virtual void addDForce(VecDeriv& df, const VecDeriv& dx);

    virtual Real getLambda() const { return lambda;}
    virtual Real getMu() const { return mu;}

    void setYoungModulus(const double modulus)
    {
        f_youngModulus.setValue((Real)modulus);
    }
    void setPoissonRatio(const double ratio)
    {
        f_poissonRatio.setValue((Real)ratio);
    }
    void includeAngularSprings(const bool useAngularSprings)
    {
        f_useAngularSprings.setValue(useAngularSprings);
    }


    // handle topological changes
    virtual void handleTopologyChange();

    // -- VisualModel interface
    void draw();
    void initTextures() { };
    void update() { };
    /// compute lambda and mu based on the Young modulus and Poisson ratio
    void updateLameCoefficients();



protected :

    EdgeData<EdgeRestInformation> &getEdgeInfo() {return edgeInfo;}

    static void TRQSEdgeCreationFunction(int edgeIndex, void* param, EdgeRestInformation &ei,
            const Edge& ,  const sofa::helper::vector< unsigned int > &,
            const sofa::helper::vector< double >&);
    static void TRQSTriangleCreationFunction (int , void* ,
            TriangleRestInformation &,
            const Triangle& , const sofa::helper::vector< unsigned int > &, const sofa::helper::vector< double >&);


    static void TRQSTriangleDestroyFunction ( int , void* , TriangleRestInformation &);

};
} //namespace forcefield

} // namespace Components


} // namespace Sofa



#endif /* _TriangularQuadraticSpringsForceField_H_ */
