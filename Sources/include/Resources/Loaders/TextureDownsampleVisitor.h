#pragma once
#include <osg/NodeVisitor>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>


struct TextureDownsampleVisitor : public osg::NodeVisitor {
    TextureDownsampleVisitor()
        : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}

    void apply(osg::Geode& geode) override {
        for (unsigned int i = 0; i < geode.getNumDrawables(); ++i) {
            osg::ref_ptr<osg::Drawable> drawable = geode.getDrawable(i);
            osg::StateSet* stateset = drawable->getStateSet();
            if (!stateset) continue;

            osg::StateSet::TextureAttributeList& texAttribList = stateset->getTextureAttributeList();
            for (unsigned int unit = 0; unit < texAttribList.size(); ++unit) {
                osg::Texture2D* tex2D = dynamic_cast<osg::Texture2D*>(stateset->getTextureAttribute(unit, osg::StateAttribute::TEXTURE));
                if (tex2D) {
                    osg::Image* image = tex2D->getImage();
                    if (image) {
                        int oldW = image->s();
                        int oldH = image->t();
                        if (oldW > 1024 || oldH > 1024) {
                            int newW = static_cast<int>(oldW * 0.125);
                            int newH = static_cast<int>(oldH * 0.125);
                            if (newW < 1) newW = 1;
                            if (newH < 1) newH = 1;

                            // 直接scale，不用再copy
                            image->scaleImage(newW, newH, image->r());
                            tex2D->dirtyTextureObject();

                            // std::cout << "[Downsampled] Texture size " << oldW << "x" << oldH << " -> " << newW << "x" << newH << std::endl;
                        }
                    }
                }
            }
        }
        traverse(geode);
    }
};
