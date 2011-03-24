/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef FRAME_MAPPINGTYPES_H
#define FRAME_MAPPINGTYPES_H

#include "FrameMass.h"
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/component/topology/PointData.h>

namespace sofa
{

namespace defaulttype
{

using sofa::defaulttype::FrameMass;

template<class In, class Out, class Material, int nbRef, int type>
class LinearBlendTypes;

template<class In, class Out, class Material, int nbRef, int type>
class DualQuatBlendTypes;



template< class Primitive>
class InDataTypesInfo
{
public:
    enum {VSize = Primitive::VSize};
};

template<int Dim, typename Real> class StdRigidTypes;

template< class Real, int Dim>
class InDataTypesInfo<StdRigidTypes<Dim,Real> >
{
public:
    enum {VSize = 6};
};



template< class Primitive>
class OutDataTypesInfo
{
public:
    typedef Vec<3,typename Primitive::Real> MaterialCoord;
    typedef vector<MaterialCoord> VecMaterialCoord;
    enum {primitive_order = 0};
    enum {type = 0};
};

template<int Spatial_dimensions, int Material_dimensions, int Order, typename Real> struct DeformationGradientTypes;

template< class Real, int Dim, int Order>
class OutDataTypesInfo<DeformationGradientTypes<Dim,Dim,Order,Real> >
{
public:
    typedef typename DeformationGradientTypes<Dim,Dim,Order,Real>::MaterialCoord MaterialCoord;
    typedef typename DeformationGradientTypes<Dim,Dim,Order,Real>::VecMaterialCoord VecMaterialCoord;
    enum {primitive_order = DeformationGradientTypes<Dim,Dim,Order,Real>::order};
    enum {type = DeformationGradientTypes<Dim,Dim,Order,Real>::order};
};


template< int N, typename real>
class OutDataTypesInfo<StdAffineTypes<N,real> >
{
public:
    typedef typename Vec<3,real> MaterialCoord;
    typedef typename vector<MaterialCoord > VecMaterialCoord;
    enum {primitive_order = 0};
    enum {type = 3};
};

template< int N, typename real>
class OutDataTypesInfo<StdRigidTypes<N,real> >
{
public:
    typedef typename Vec<3,real> MaterialCoord;
    typedef typename vector<MaterialCoord > VecMaterialCoord;
    enum {primitive_order = 0};
    enum {type = 4};
};

template< int N, typename real>
class OutDataTypesInfo<StdQuadraticTypes<N,real> >
{
public:
    typedef typename Vec<3,real> MaterialCoord;
    typedef typename vector<MaterialCoord > VecMaterialCoord;
    enum {primitive_order = 0};
    enum {type = 5};
};


template<class TIn, bool IsPhysical>
class FrameData : public  virtual core::objectmodel::BaseObject
{
public:
    // Input types
    typedef TIn In;
    typedef typename In::Real InReal;
    static const bool isPhysical = IsPhysical;
    static const unsigned int num_spatial_dimensions=In::spatial_dimensions;
    enum {InVSize= defaulttype::InDataTypesInfo<In>::VSize};
    typedef FrameMass<num_spatial_dimensions,InVSize,InReal> FrameMassType;
    typedef sofa::component::topology::PointData<FrameMassType> VecMass;
    typedef helper::vector<FrameMassType> MassVector;

    FrameData() {}
    virtual void LumpMassesToFrames (MassVector& f_mass0, MassVector& f_mass) = 0;
};



template<class TOut>
class SampleData : public  virtual core::objectmodel::BaseObject
{
public:
    // Output types
    typedef TOut Out;
    typedef typename OutDataTypesInfo<Out>::MaterialCoord MaterialCoord;
    typedef typename OutDataTypesInfo<Out>::VecMaterialCoord VecMaterialCoord;

    Data<VecMaterialCoord> f_materialPoints;

    SampleData()
        : f_materialPoints ( initData ( &f_materialPoints,"materialPoints","Coordinates of the samples in object space" ) )
    {
    }

    virtual void apply( MaterialCoord& coord, const MaterialCoord& restCoord) = 0; // Allow to tranfsorm a voxel from restPos for example
    //virtual void addSamples( const unsigned int& nbNewVertices) = 0;
    //virtual void removeSamples( const vector<unsigned int>& samplesID) = 0;
};

} // namespace defaulttype
} // namespace sofa



#endif
