#include "EldoraProducts.h"
#include <iostream>

#define TOSHORT(data, gain, offset) ((short)(((data)*(gain))-(offset)))

////////////////////////////////////////////////////
EldoraProducts::EldoraProducts(
        DDSPublisher& publisher,
            std::string productsTopic) :
    _rays(0), _publisher(publisher), _productsTopic(productsTopic),
            _productsWriter(publisher, productsTopic), _droppedRays(0) {
            
}

////////////////////////////////////////////////////
EldoraProducts::~EldoraProducts() {

}

////////////////////////////////////////////////////
void EldoraProducts::newRayData(
        std::vector<EldoraDDS::Ray*>& rays) {

    _rays++;

    // We have four matched rays. 
    Products* products = _productsWriter.getEmptyItem();
    if (products) {
        
        // initiallize the fixed fields in Products.
        initProducts(products);
        
        // transfer beam metadata
        products->radarId = rays[0]->radarId;
        products->timestamp = rays[0]->hskp.rayNum;
        products->elDegrees = rays[0]->hskp.elevation;
        
        // The abp rays are three times as long as the
        // product beams. 
        int productsLength = rays[0]->abp.length()/3;
        // Resize the beam lengths.
        products->p1.length(productsLength);
        products->p2.length(productsLength);
        products->p3.length(productsLength);
        products->p4.length(productsLength);
        products->vr.length(productsLength);
        products->vs.length(productsLength);
        products->vl.length(productsLength);
        products->dm.length(productsLength);
        products->dbz.length(productsLength);
        products->sw.length(productsLength);
        products->ncp.length(productsLength);
        
        // Compute the products.
         for (int i = 0; i < productsLength; i++) {
             int p = 3*i;
            // temprary hack: Sum all four channel powers to create
            // dbz.
            double dbz = 0.0;
            for (unsigned int f = 0; f < 4; f++) {
                dbz += rays[f]->abp[p+2];
            }
            
            products->dm[i] = TOSHORT(dbz/(((i+1.0)*(i+1.0))/100.0), products->dmGain, products->dmOffset);
                
            products->dbz[i] = TOSHORT(dbz, products->dbzGain, products->dbzOffset)*100-70;

            /** 
            if (products->radarId == EldoraDDS::Aft) {
				if (i >= 45 && i <= 60) {
					std::cout << "ABP[2][" << i << "] "
					<< rays[0]->abp[p+2] << " "
					<< rays[1]->abp[p+2] << " "
					<< rays[2]->abp[p+2] << " "
					<< rays[3]->abp[p+2] << "\n";
				}
			}
			**/
            products->p1[i] = TOSHORT(rays[0]->abp[p+2], products->p1Gain, products->p1Offset);
            products->p2[i] = TOSHORT(rays[1]->abp[p+2], products->p2Gain, products->p2Offset);
            products->p3[i] = TOSHORT(rays[2]->abp[p+2], products->p3Gain, products->p3Offset);
            products->p4[i] = TOSHORT(rays[3]->abp[p+2], products->p4Gain, products->p4Offset);
           
            products->vr[i] = TOSHORT((i-productsLength/2.0), products->vrGain, products->vrOffset);
            products->vs[i] = TOSHORT((i-productsLength/2.0)/10.0, products->vsGain, products->vsOffset);
            products->vl[i] = TOSHORT(i*(i-productsLength/2.0)/productsLength, products->vlGain, products->vlOffset);
            products->sw[i] = TOSHORT(1.0*i, products->swGain, products->swOffset);
            products->ncp[i] = TOSHORT(1.0/((i+1.0)*(i+1.0)), products->ncpGain, products->ncpOffset);
        }
        // Publish the products.
        _productsWriter.publishItem(products);
    } else {
        // Oh no, we couldn't get a free products item, so
        // we have to ignore this ray.
        _droppedRays++;
    }
    return;
}
////////////////////////////////////////////////////

int EldoraProducts::numRays() {
    int n = _rays;
    _rays = 0;
    return n;
}
////////////////////////////////////////////////////
void EldoraProducts::initProducts(EldoraDDS::Products* p) {
    
    p->p1Gain = 1.0e2;
    p->p1Offset = 9000.0;
    
    p->p2Gain = 1.0e2;
    p->p2Offset = 9000.0;
    
    p->p3Gain = 1.0e2;
    p->p3Offset = 9000.0;
    
    p->p4Gain = 1.0e2;
    p->p4Offset = 9000.0;
    
    p->dmGain = 1000.0;
    p->dmOffset = 0.0;
    
    p->dbzGain = 1000.0;
    p->dbzOffset = 0.0;
    
    p->vrGain = 100.0;
    p->vrOffset = 0.0;
    
    p->vsGain = 100.0;
    p->vsOffset = 0.0;
    
    p->vlGain = 100.0;
    p->vlOffset = 0.0;
     
    p->swGain = 100.0;
    p->swOffset = 32768.0;
    
    p->ncpGain = 1000.0;
    p->ncpOffset = 32768.0;
}
////////////////////////////////////////////////////

