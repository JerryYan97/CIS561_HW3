#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camera.h"
#include "scene.h"
#include "geometry.h"
#include "smoothblend.h"
#include "light.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include "sampler.h"
QString dir_name;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setValue(0);

    connect(ui->actionSelect_Path, SIGNAL(triggered(bool)), this, SLOT(on_dirSelected_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dirSelected_clicked()
{
    dir_name = QFileDialog::getExistingDirectory(this, QString("Select Dir"));

    ui->label_Path->setText(dir_name);
}

// Render Button
void MainWindow::on_pushButton_clicked()
{
    if(dir_name.isEmpty())
    {
        QMessageBox::information(this, "NOTE", "Please select a direction path first.");
    }
    else
    {
        // Init scene:
        QImage image(400, 400, QImage::Format_RGB32);
        Camera mainCamera = Camera(400, 400, Point3f(4, 5.5, 5), Point3f(0, 0, 0), Vector3f(0, 1, 0));
        Scene scene;
        scene.mCamera = mainCamera;

        /*Sampler mSampler(100, GRID, NONE);
        for(int i = 0; i < mSampler.samples.size(); i++)
        {
            std::cout << mSampler.samples[i].x << " " << mSampler.samples[i].y << " " << mSampler.samples[i].z << std::endl;
        }*/



        // PointLight mLight2 = PointLight(Point3f(2, 4, 2), Color3f(255, 255, 255));
        // scene.mLightList.push_back(mkU<PointLight>(mLight2));

        //Init Material:
        LambertMaterial white_diffuse = LambertMaterial(Color3f(1, 1, 1));
        LambertMaterial red_diffuse = LambertMaterial(Color3f(1, 0, 0));
        LambertMaterial green_diffuse = LambertMaterial(Color3f(0, 1, 0));
        LambertMaterial yellow_diffuse = LambertMaterial(Color3f(1, 1, 0.25));
        LambertMaterial brown_diffuse = LambertMaterial(Color3f(0.4375, 0.2578, 0.078)); // 112, 66, 20 -- 0.4375, 0.2578, 0.078
        LambertMaterial purple_specular = LambertMaterial(Color3f(0.75, 0.0, 0.75));
        LambertMaterial mirrored_specular = LambertMaterial(Color3f(1,1,1));
        LambertMaterial mirrored_specular2 = LambertMaterial(Color3f(0.25, 0.5, 1));
        LambertMaterial refractive_specular = LambertMaterial(Color3f(1, 1, 1));
        LambertMaterial air_bubble_mat = LambertMaterial(Color3f(1, 1, 1));
        LambertMaterial refractive_specular2 = LambertMaterial(Color3f(0.15, 1, 0.15));
        LightMaterial emissive_material = LightMaterial(Color3f(1, 1, 1));
        RandomMaterial rand_material = RandomMaterial(Color3f(0.75, 0.0, 0.75));



        // AreaLight mLight1 = ;
        uPtr<Primitive> primAreaLight = mkU<Primitive>(Primitive());

        uPtr<Shape> lightShapePtr = mkU<Disc>(Disc());
        Transform transLightShape = Transform(Vector3f(0, 3.5, 0), Vector3f(90, 0, 0), Vector3f(2, 2, 1));
        lightShapePtr->mTransform = transLightShape;
        scene.mLightList.push_back(mkU<AreaLight>(AreaLight(lightShapePtr.get(), LightShape::DISC, Color3f(255, 255, 255))));
        primAreaLight->mUPtrMaterial = mkU<LightMaterial>(emissive_material);
        primAreaLight->mUPtrShape = std::move(lightShapePtr);
        scene.mPrimitiveList.push_back(std::move(primAreaLight));



        // Back Wall:
        /*uPtr<Primitive> primSquarePlaneBackWall = mkU<Primitive>(Primitive());
        Transform squarePlaneTransformBackWall = Transform(Vector3f(0, 0, -5), Vector3f(0, 0, 0), Vector3f(10, 10, 1));
        uPtr<Material> mMaterialBackWall = mkU<LambertMaterial>(LambertMaterial(Color3f(1, 1, 1)));
        primSquarePlaneBackWall->mUPtrMaterial = std::move(mMaterialBackWall);

        primSquarePlaneBackWall->mName = QString("Back Wall");
        primSquarePlaneBackWall->mUPtrShape = mkU<SquarePlane>(SquarePlane());
        primSquarePlaneBackWall->mUPtrShape->mTransform = squarePlaneTransformBackWall;
        scene.mPrimitiveList.push_back(std::move(primSquarePlaneBackWall));


        // Right Wall:
        uPtr<Primitive> primSquarePlaneRightWall = mkU<Primitive>(Primitive());
        Transform squarePlaneTransformRightWall = Transform(Vector3f(5, 0, 0), Vector3f(0, -90, 0), Vector3f(10, 10, 1));
        uPtr<Material> mMaterialRightWall = mkU<LambertMaterial>(LambertMaterial(Color3f(0, 1, 0)));
        primSquarePlaneRightWall->mUPtrMaterial = std::move(mMaterialRightWall);

        primSquarePlaneRightWall->mName = QString("Right Wall");
        primSquarePlaneRightWall->mUPtrShape = mkU<SquarePlane>(SquarePlane());
        primSquarePlaneRightWall->mUPtrShape->mTransform = squarePlaneTransformRightWall;
        scene.mPrimitiveList.push_back(std::move(primSquarePlaneRightWall));


        // Left Wall:
        uPtr<Primitive> primSquarePlaneLeftWall = mkU<Primitive>(Primitive());
        Transform squarePlaneTransformLeftWall = Transform(Vector3f(-5, 0, 0), Vector3f(0, 90, 0), Vector3f(10, 10, 1));
        uPtr<Material> mMaterialLeftWall = mkU<LambertMaterial>(LambertMaterial(Color3f(1, 0, 0)));
        primSquarePlaneLeftWall->mUPtrMaterial = std::move(mMaterialLeftWall);

        primSquarePlaneLeftWall->mName = QString("Left Wall");
        primSquarePlaneLeftWall->mUPtrShape = mkU<SquarePlane>(SquarePlane());
        primSquarePlaneLeftWall->mUPtrShape->mTransform = squarePlaneTransformLeftWall;
        scene.mPrimitiveList.push_back(std::move(primSquarePlaneLeftWall));

        // Ceiling Wall:
        uPtr<Primitive> primSquarePlaneCeiling = mkU<Primitive>(Primitive());
        Transform squarePlaneTransformCeiling = Transform(Vector3f(0, 5, 0), Vector3f(90, 0, 0), Vector3f(10, 10, 1));
        uPtr<Material> mMaterialCeiling = mkU<LambertMaterial>(LambertMaterial(Color3f(1, 1, 1)));
        primSquarePlaneCeiling->mUPtrMaterial = std::move(mMaterialCeiling);

        primSquarePlaneCeiling->mName = QString("Ceiling Wall");
        primSquarePlaneCeiling->mUPtrShape = mkU<SquarePlane>(SquarePlane());
        primSquarePlaneCeiling->mUPtrShape->mTransform = squarePlaneTransformCeiling;
        scene.mPrimitiveList.push_back(std::move(primSquarePlaneCeiling));


        // Floor Wall:
        uPtr<Primitive> primSquarePlaneFloor = mkU<Primitive>(Primitive());
        Transform squarePlaneTransformFloor = Transform(Vector3f(0, -5, 0), Vector3f(-90, 0, 0), Vector3f(10, 10, 1));
        uPtr<Material> mMaterialFloor = mkU<LambertMaterial>(LambertMaterial(Color3f(1, 1, 1)));
        primSquarePlaneFloor->mUPtrMaterial = std::move(mMaterialFloor);

        primSquarePlaneFloor->mName = QString("Floor Wall");
        primSquarePlaneFloor->mUPtrShape = mkU<SquarePlane>(SquarePlane());
        primSquarePlaneFloor->mUPtrShape->mTransform = squarePlaneTransformFloor;
        scene.mPrimitiveList.push_back(std::move(primSquarePlaneFloor));*/

        // Test Scene:
        // Floor Wall:
        uPtr<Primitive> primSquarePlaneFloor = mkU<Primitive>(Primitive());
        Transform squarePlaneTransformFloor = Transform(Vector3f(0, 0, 0), Vector3f(-90, 0, 0), Vector3f(10, 10, 1));
        uPtr<Material> mMaterialFloor = mkU<LambertMaterial>(LambertMaterial(Color3f(1, 1, 1)));
        primSquarePlaneFloor->mUPtrMaterial = std::move(mMaterialFloor);

        primSquarePlaneFloor->mName = QString("Floor Wall");
        primSquarePlaneFloor->mUPtrShape = mkU<SquarePlane>(SquarePlane());
        primSquarePlaneFloor->mUPtrShape->mTransform = squarePlaneTransformFloor;
        scene.mPrimitiveList.push_back(std::move(primSquarePlaneFloor));


        uPtr<Primitive> primShinySphere4 = mkU<Primitive>(Primitive());
        Transform transShinySphere4 = Transform(Vector3f(0.f, 1.f, 0.f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primShinySphere4->mUPtrMaterial = mkU<LambertMaterial>(purple_specular);

        primShinySphere4->mName = QString("Shiny Sphere");
        primShinySphere4->mUPtrShape = mkU<Sphere>(Sphere());
        primShinySphere4->mUPtrShape->mTransform = transShinySphere4;
        scene.mPrimitiveList.push_back(std::move(primShinySphere4));


        /*
        uPtr<Primitive> primBlend = mkU<Primitive>(Primitive());
        uPtr<SmoothBlend> mS1S2 = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mS3S4 = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mFinal = mkU<SmoothBlend>(SmoothBlend());
        mS1S2->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mS3S4->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mFinal->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));

        Transform transShinySphere1 = Transform(Vector3f(-0.25f, 1, -0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        Transform transShinySphere2 = Transform(Vector3f(0.25f, 1, -0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        Transform transShinySphere3 = Transform(Vector3f(-0.25f, 1, 0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        Transform transShinySphere4 = Transform(Vector3f(0.25f, 1, 0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));

        uPtr<Sphere> mS1 = mkU<Sphere>(Sphere());
        uPtr<Sphere> mS2 = mkU<Sphere>(Sphere());
        uPtr<Sphere> mS3 = mkU<Sphere>(Sphere());
        uPtr<Sphere> mS4 = mkU<Sphere>(Sphere());
        mS1->mTransform = transShinySphere1;
        mS2->mTransform = transShinySphere2;
        mS3->mTransform = transShinySphere3;
        mS4->mTransform = transShinySphere4;

        mS1S2->mShape1 = mS1.get();
        mS1S2->mShape2 = mS2.get();
        mS3S4->mShape1 = mS3.get();
        mS3S4->mShape2 = mS4.get();
        mFinal->mShape1 = mS1S2.get();
        mFinal->mShape2 = mS3S4.get();

        primBlend->mUPtrMaterial = mkU<RandomMaterial>(rand_material);
        primBlend->mName = QString("Shiny Blend");
        primBlend->mUPtrShape = std::move(mFinal);
        scene.mPrimitiveList.push_back(std::move(primBlend));
        */
        // Sphere1:
        /*
        uPtr<Primitive> primShinySphere1 = mkU<Primitive>(Primitive());
        Transform transShinySphere1 = Transform(Vector3f(-0.25f, 1, -0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primShinySphere1->mUPtrMaterial = mkU<RandomMaterial>(rand_material);

        primShinySphere1->mName = QString("Shiny Sphere");
        primShinySphere1->mUPtrShape = mkU<Sphere>(Sphere());
        primShinySphere1->mUPtrShape->mTransform = transShinySphere1;
        scene.mPrimitiveList.push_back(std::move(primShinySphere1));

        // Sphere2:
        uPtr<Primitive> primShinySphere2 = mkU<Primitive>(Primitive());
        Transform transShinySphere2 = Transform(Vector3f(0.25f, 1, -0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primShinySphere2->mUPtrMaterial = mkU<RandomMaterial>(rand_material);

        primShinySphere2->mName = QString("Shiny Sphere");
        primShinySphere2->mUPtrShape = mkU<Sphere>(Sphere());
        primShinySphere2->mUPtrShape->mTransform = transShinySphere2;
        scene.mPrimitiveList.push_back(std::move(primShinySphere2));

        // Sphere3:
        uPtr<Primitive> primShinySphere3 = mkU<Primitive>(Primitive());
        Transform transShinySphere3 = Transform(Vector3f(-0.25f, 1, 0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primShinySphere3->mUPtrMaterial = mkU<RandomMaterial>(rand_material);

        primShinySphere3->mName = QString("Shiny Sphere");
        primShinySphere3->mUPtrShape = mkU<Sphere>(Sphere());
        primShinySphere3->mUPtrShape->mTransform = transShinySphere3;
        scene.mPrimitiveList.push_back(std::move(primShinySphere3));

        // Sphere4:
        uPtr<Primitive> primShinySphere4 = mkU<Primitive>(Primitive());
        Transform transShinySphere4 = Transform(Vector3f(0.25f, 1, 0.25f), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primShinySphere4->mUPtrMaterial = mkU<RandomMaterial>(rand_material);

        primShinySphere4->mName = QString("Shiny Sphere");
        primShinySphere4->mUPtrShape = mkU<Sphere>(Sphere());
        primShinySphere4->mUPtrShape->mTransform = transShinySphere4;
        scene.mPrimitiveList.push_back(std::move(primShinySphere4));
        */
        // Disc:
        /*uPtr<Primitive> primDisc = mkU<Primitive>(Primitive());
        Transform transDisc = Transform(Vector3f(0, -2, 0), Vector3f(-90, 0, 0), Vector3f(1, 1, 1));
        uPtr<Material> mMaterialDisc = mkU<LambertMaterial>(green_diffuse);
        primDisc->mUPtrMaterial = std::move(mMaterialDisc);

        primDisc->mName = QString("Disc");
        primDisc->mUPtrShape = mkU<Disc>(Disc());
        primDisc->mUPtrShape->mTransform = transDisc;
        scene.mPrimitiveList.push_back(std::move(primDisc));*/

        // Spong Bob:
        // Spong Bob's yellow body:
        /*uPtr<Primitive> primBody = mkU<Primitive>(Primitive());
        Transform bodyTransform = Transform(Vector3f(0, 0, -2), Vector3f(0, 0, 0), Vector3f(3, 4.5, 1));
        uPtr<Material> materialBody = mkU<LambertMaterial>(yellow_diffuse);
        primBody->mUPtrMaterial = std::move(materialBody);

        primBody->mName = QString("body");
        primBody->mUPtrShape = mkU<Box>(Box());
        primBody->mUPtrShape->mTransform = bodyTransform;
        scene.mPrimitiveList.push_back(std::move(primBody));

        // Spong Bob's left shoe:
        uPtr<Primitive> primLShoe = mkU<Primitive>(Primitive());
        Transform lShoeTransform = Transform(Vector3f(1, -4.3, -2), Vector3f(0, 0, 0), Vector3f(1.5, 1, 2));
        uPtr<Material> materialLShoe = mkU<LambertMaterial>(brown_diffuse);
        primLShoe->mUPtrMaterial = std::move(materialLShoe);

        primLShoe->mName = QString("L Shoe");
        primLShoe->mUPtrShape = mkU<Box>(Box());
        primLShoe->mUPtrShape->mTransform = lShoeTransform ;
        scene.mPrimitiveList.push_back(std::move(primLShoe));

        // Spong Bob's right shoe:
        uPtr<Primitive> primRShoe = mkU<Primitive>(Primitive());
        Transform rShoeTransform = Transform(Vector3f(-1, -4.3, -2), Vector3f(0, 0, 0), Vector3f(1.5, 1, 2));
        uPtr<Material> materialRShoe = mkU<LambertMaterial>(brown_diffuse);
        primRShoe->mUPtrMaterial = std::move(materialRShoe);

        primRShoe->mName = QString("R Shoe");
        primRShoe->mUPtrShape = mkU<Box>(Box());
        primRShoe->mUPtrShape->mTransform = rShoeTransform ;
        scene.mPrimitiveList.push_back(std::move(primRShoe));

        // Spong Bob's left leg:
        uPtr<Primitive> primLLeg = mkU<Primitive>(Primitive());
        Transform lLegTransform = Transform(Vector3f(1, -2, -2), Vector3f(0, 0, 0), Vector3f(1, 4, 1));
        uPtr<Material> materialLLeg = mkU<LambertMaterial>(yellow_diffuse);
        primLLeg->mUPtrMaterial = std::move(materialLLeg);

        primLLeg->mName = QString("L Leg");
        primLLeg->mUPtrShape = mkU<Capsule>(Capsule());
        primLLeg->mUPtrShape->mTransform = lLegTransform;
        scene.mPrimitiveList.push_back(std::move(primLLeg));

        // Spong Bob's right leg:
        uPtr<Primitive> primRLeg = mkU<Primitive>(Primitive());
        Transform rLegTransform = Transform(Vector3f(-1, -2, -2), Vector3f(0, 0, 0), Vector3f(1, 4, 1));
        uPtr<Material> materialRLeg = mkU<LambertMaterial>(yellow_diffuse);
        primRLeg->mUPtrMaterial = std::move(materialRLeg);

        primRLeg->mName = QString("R Leg");
        primRLeg->mUPtrShape = mkU<Capsule>(Capsule());
        primRLeg->mUPtrShape->mTransform = rLegTransform;
        scene.mPrimitiveList.push_back(std::move(primRLeg));


        // Spong Bob's left arm:
        uPtr<Primitive> primLArm = mkU<Primitive>(Primitive());
        Transform lArmTransform = Transform(Vector3f(1, -0.2, -2), Vector3f(90, 0, 90), Vector3f(1, 4, 1));
        uPtr<Material> materialLArm = mkU<LambertMaterial>(yellow_diffuse);
        primLArm->mUPtrMaterial = std::move(materialLArm);

        primLArm->mName = QString("L Arm");
        primLArm->mUPtrShape = mkU<Capsule>(Capsule());
        primLArm->mUPtrShape->mTransform = lArmTransform;
        scene.mPrimitiveList.push_back(std::move(primLArm));

        // Spong Bob's right arm:
        uPtr<Primitive> primRArm = mkU<Primitive>(Primitive());
        Transform rArmTransform = Transform(Vector3f(-1, -0.2, -2), Vector3f(90, 0, 90), Vector3f(1, 4, 1));
        uPtr<Material> materialRArm = mkU<LambertMaterial>(yellow_diffuse);
        primRArm->mUPtrMaterial = std::move(materialRArm);

        primRArm->mName = QString("R Arm");
        primRArm->mUPtrShape = mkU<Capsule>(Capsule());
        primRArm->mUPtrShape->mTransform = rArmTransform;
        scene.mPrimitiveList.push_back(std::move(primRArm));

        // Spong Bob's left hand:
        uPtr<Primitive> primLHand = mkU<Primitive>(Primitive());
        Transform transLHand = Transform(Vector3f(3.5, -0.2, -1.8), Vector3f(0, 0, 0), Vector3f(0.8, 0.8, 0.8));
        primLHand->mUPtrMaterial = mkU<LambertMaterial>(yellow_diffuse);

        primLHand->mName = QString("Left Hand");
        primLHand->mUPtrShape = mkU<Sphere>(Sphere());
        primLHand->mUPtrShape->mTransform = transLHand;
        scene.mPrimitiveList.push_back(std::move(primLHand));

        // Spong Bob's right hand:
        uPtr<Primitive> primRHand = mkU<Primitive>(Primitive());
        Transform transRHand = Transform(Vector3f(-3.5, -0.2, -1.8), Vector3f(0, 0, 0), Vector3f(0.8, 0.8, 0.8));
        primRHand->mUPtrMaterial = mkU<LambertMaterial>(yellow_diffuse);

        primRHand->mName = QString("Right Hand");
        primRHand->mUPtrShape = mkU<Sphere>(Sphere());
        primRHand->mUPtrShape->mTransform = transRHand;
        scene.mPrimitiveList.push_back(std::move(primRHand));


        // Spong Bob's left eye:
        // uPtr<Primitive> primLEye = mkU<Primitive>(Primitive());
        uPtr<Shape> lEyeShapePtr = mkU<Sphere>(Sphere()); // *
        Transform transLEye = Transform(Vector3f(0.5, 0.5, -1.8), Vector3f(0, 0, 0), Vector3f(1.5, 1.5, 1.5));
        lEyeShapePtr->mTransform = transLEye; // *
        // primLEye->mUPtrMaterial = mkU<LambertMaterial>(white_diffuse);

        // primLEye->mName = QString("Left Eye");
        // primLEye->mUPtrShape = mkU<Sphere>(Sphere());
        // primLEye->mUPtrShape->mTransform = transLEye;
        // scene.mPrimitiveList.push_back(std::move(primLEye));

        // Spong Bob's right eye:
        // uPtr<Primitive> primREye = mkU<Primitive>(Primitive());
        uPtr<Shape> rEyeShapePtr = mkU<Sphere>(Sphere()); // *
        Transform transREye = Transform(Vector3f(-0.5, 0.5, -1.8), Vector3f(0, 0, 0), Vector3f(1.5, 1.5, 1.5));
        rEyeShapePtr->mTransform = transREye; // *
        // primREye->mUPtrMaterial = mkU<LambertMaterial>(white_diffuse);

        // primREye->mName = QString("Right Eye");
        // primREye->mUPtrShape = mkU<Sphere>(Sphere());
        // primREye->mUPtrShape->mTransform = transREye;
        // scene.mPrimitiveList.push_back(std::move(primREye));
        */


        // Smooth Blend:
        // Smooth blend of Spong Bob's eyes:
        // Create BlendSmooth primitive:
        /*uPtr<Primitive> primSbEyes = mkU<Primitive>(Primitive());
        primSbEyes->mName = QString("Blend Smooth Eyes");

        uPtr<Material> sbEyesMaterial = mkU<LambertMaterial>(LambertMaterial(Color3f(1, 1, 1)));
        // uPtr<Material> mMaterial = mkU<BlinnPhongMaterial>(BlinnPhongMaterial(Color3f(1, 1, 1), 5.f));
        primSbEyes->mUPtrMaterial = std::move(sbEyesMaterial);

        uPtr<Shape> lEyeShapePtr = mkU<Sphere>(Sphere());
        uPtr<Shape> rEyeShapePtr = mkU<Sphere>(Sphere());
        Transform transLEye = Transform(Vector3f(0.5, 0.5, -1.8), Vector3f(0, 0, 0), Vector3f(1.5, 1.5, 1.5));
        Transform transREye = Transform(Vector3f(-0.5, 0.5, -1.8), Vector3f(0, 0, 0), Vector3f(1.5, 1.5, 1.5));
        lEyeShapePtr->mTransform = transLEye;
        rEyeShapePtr->mTransform = transREye;

        uPtr<SmoothBlend> mSbEyesPtr = mkU<SmoothBlend>(SmoothBlend());
        mSbEyesPtr->mShape1 = rEyeShapePtr.get();
        mSbEyesPtr->mShape2 = lEyeShapePtr.get();
        mSbEyesPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primSbEyes->mUPtrShape = std::move(mSbEyesPtr);

        scene.mPrimitiveList.push_back(std::move(primSbEyes));



        // Smooth blend of Spong Bob's arms, legs, body and shoes:
        uPtr<Primitive> primSbBody = mkU<Primitive>(Primitive());
        primSbBody->mName = QString("Blend Smooth Body");

        uPtr<Material> sbBodyMaterial = mkU<LambertMaterial>(yellow_diffuse);
        // uPtr<Material> mMaterial = mkU<BlinnPhongMaterial>(BlinnPhongMaterial(Color3f(1, 1, 1), 5.f));
        primSbBody->mUPtrMaterial = std::move(sbBodyMaterial);

        uPtr<SmoothBlend> mSbArmsPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mSbLegsPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mSbHandsPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mSbShoesPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mSbMixArmsLegsPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mSbMixHandsBodyPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mSbMixHandsBodyShoesPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<SmoothBlend> mSbMixFinalPtr = mkU<SmoothBlend>(SmoothBlend());
        mSbArmsPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mSbLegsPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mSbHandsPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mSbShoesPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mSbMixArmsLegsPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mSbMixHandsBodyPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mSbMixFinalPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        mSbMixHandsBodyShoesPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));


        Transform transRHand = Transform(Vector3f(-3.5, -0.2, -1.8), Vector3f(0, 0, 0), Vector3f(0.8, 0.8, 0.8));
        Transform transLHand = Transform(Vector3f(3.5, -0.2, -1.8), Vector3f(0, 0, 0), Vector3f(0.8, 0.8, 0.8));
        Transform rArmTransform = Transform(Vector3f(-1, -0.2, -2), Vector3f(90, 0, 90), Vector3f(1, 4, 1));
        Transform lArmTransform = Transform(Vector3f(1, -0.2, -2), Vector3f(90, 0, 90), Vector3f(1, 4, 1));
        Transform rLegTransform = Transform(Vector3f(-1, -2, -2), Vector3f(0, 0, 0), Vector3f(1, 4, 1));
        Transform lLegTransform = Transform(Vector3f(1, -2, -2), Vector3f(0, 0, 0), Vector3f(1, 4, 1));
        Transform rShoeTransform = Transform(Vector3f(-1, -4.3, -2), Vector3f(0, 0, 0), Vector3f(1.5, 1, 2));
        Transform lShoeTransform = Transform(Vector3f(1, -4.3, -2), Vector3f(0, 0, 0), Vector3f(1.5, 1, 2));
        Transform bodyTransform = Transform(Vector3f(0, 0, -2), Vector3f(0, 0, 0), Vector3f(3, 4.5, 1));


        uPtr<Shape> rHandShapePtr = mkU<Sphere>(Sphere());
        uPtr<Shape> lHandShapePtr = mkU<Sphere>(Sphere());
        uPtr<Shape> rArmShapePtr = mkU<Capsule>(Capsule());
        uPtr<Shape> lArmShapePtr = mkU<Capsule>(Capsule());
        uPtr<Shape> rLegShapePtr = mkU<Capsule>(Capsule());
        uPtr<Shape> lLegShapePtr = mkU<Capsule>(Capsule());
        uPtr<Shape> rShoeShapePtr = mkU<Box>(Box());
        uPtr<Shape> lShoeShapePtr = mkU<Box>(Box());
        uPtr<Shape> bodyShapePtr = mkU<Box>(Box());

        rHandShapePtr->mTransform = transRHand;
        lHandShapePtr->mTransform = transLHand;
        rArmShapePtr->mTransform = rArmTransform;
        lArmShapePtr->mTransform = lArmTransform;
        rLegShapePtr->mTransform = rLegTransform;
        lLegShapePtr->mTransform = lLegTransform;
        rShoeShapePtr->mTransform = rShoeTransform;
        lShoeShapePtr->mTransform = lShoeTransform;
        bodyShapePtr->mTransform = bodyTransform;

        mSbArmsPtr->mShape1 = rArmShapePtr.get();
        mSbArmsPtr->mShape2 = lArmShapePtr.get();
        mSbLegsPtr->mShape1 = rLegShapePtr.get();
        mSbLegsPtr->mShape2 = lLegShapePtr.get();
        mSbHandsPtr->mShape1 = rHandShapePtr.get();
        mSbHandsPtr->mShape2 = lHandShapePtr.get();
        mSbShoesPtr->mShape1 = rShoeShapePtr.get();
        mSbShoesPtr->mShape2 = lShoeShapePtr.get();
        mSbMixArmsLegsPtr->mShape1 = mSbArmsPtr.get();
        mSbMixArmsLegsPtr->mShape2 = mSbLegsPtr.get();
        mSbMixHandsBodyPtr->mShape1 = mSbHandsPtr.get();
        mSbMixHandsBodyPtr->mShape2 = bodyShapePtr.get();
        mSbMixHandsBodyShoesPtr->mShape1 = mSbShoesPtr.get();
        mSbMixHandsBodyShoesPtr->mShape2 = mSbMixHandsBodyPtr.get();
        mSbMixFinalPtr->mShape1 = mSbMixArmsLegsPtr.get();
        mSbMixFinalPtr->mShape2 = mSbMixHandsBodyShoesPtr.get();

        primSbBody->mUPtrShape = std::move(mSbMixFinalPtr);

        scene.mPrimitiveList.push_back(std::move(primSbBody));*/



        // Spong Bob's right eye:
        /*uPtr<Primitive> primREye = mkU<Primitive>(Primitive());
        Transform transREye = Transform(Vector3f(-0.5, 0, -2.5), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primREye->mUPtrMaterial = mkU<LambertMaterial>(white_diffuse);

        primREye->mName = QString("Right Eye");
        primREye->mUPtrShape = mkU<Sphere>(Sphere());
        primREye->mUPtrShape->mTransform = transREye;
        scene.mPrimitiveList.push_back(std::move(primREye));*/


        // Spong Bob's right eye:
        /*uPtr<Primitive> primREye = mkU<Primitive>(Primitive());
        Transform transREye = Transform(Vector3f(-0.5, 0, -2.5), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primREye->mUPtrMaterial = mkU<LambertMaterial>(white_diffuse);

        primREye->mName = QString("Right Eye");
        primREye->mUPtrShape = mkU<Sphere>(Sphere());
        primREye->mUPtrShape->mTransform = transREye;
        scene.mPrimitiveList.push_back(std::move(primREye));*/




        // Standard Scene:
        // Shiny Sphere:
        /*uPtr<Primitive> primShinySphere = mkU<Primitive>(Primitive());
        Transform transShinySphere = Transform(Vector3f(-2, -5, 0), Vector3f(0, 0, 0), Vector3f(5, 5, 5));
        primShinySphere->mUPtrMaterial = mkU<LambertMaterial>(purple_specular);

        primShinySphere->mName = QString("Shiny Sphere");
        primShinySphere->mUPtrShape = mkU<Sphere>(Sphere());
        primShinySphere->mUPtrShape->mTransform = transShinySphere;
        scene.mPrimitiveList.push_back(std::move(primShinySphere));

        // Trans1:
        uPtr<Primitive> primTransSphere1 = mkU<Primitive>(Primitive());
        Transform transTransSphere1 = Transform(Vector3f(-3, 0, 2), Vector3f(0, 0, 0), Vector3f(4, 4, 4));
        primTransSphere1->mUPtrMaterial = mkU<LambertMaterial>(refractive_specular);

        primTransSphere1->mName = QString("TransSphere1");
        primTransSphere1->mUPtrShape = mkU<Sphere>(Sphere());
        primTransSphere1->mUPtrShape->mTransform = transTransSphere1;
        scene.mPrimitiveList.push_back(std::move(primTransSphere1));


        // Trans2:
        uPtr<Primitive> primTransSphere2 = mkU<Primitive>(Primitive());
        Transform transTransSphere2 = Transform(Vector3f(-3, 0, 2), Vector3f(0, 0, 0), Vector3f(2.5, 2.5, 2.5));
        primTransSphere2->mUPtrMaterial = mkU<LambertMaterial>(air_bubble_mat);

        primTransSphere2->mName = QString("TransSphere2");
        primTransSphere2->mUPtrShape = mkU<Sphere>(Sphere());
        primTransSphere2->mUPtrShape->mTransform = transTransSphere2;
        scene.mPrimitiveList.push_back(std::move(primTransSphere2));


        // TransSphereGreen:
        uPtr<Primitive> primTransSphereGreen = mkU<Primitive>(Primitive());
        Transform transTransSphereGreen = Transform(Vector3f(3, -2, -3), Vector3f(0, 0, 0), Vector3f(4, 4, 4));
        primTransSphereGreen->mUPtrMaterial = mkU<LambertMaterial>(refractive_specular2);

        primTransSphereGreen->mName = QString("Transparent Sphere Green");
        primTransSphereGreen->mUPtrShape = mkU<Sphere>(Sphere());
        primTransSphereGreen->mUPtrShape->mTransform = transTransSphereGreen;
        scene.mPrimitiveList.push_back(std::move(primTransSphereGreen));


        // Inside Opaque 1:
        uPtr<Primitive> primInsideOpaque1 = mkU<Primitive>(Primitive());
        Transform transInsideOpaque1 = Transform(Vector3f(3, -2, -3), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primInsideOpaque1->mUPtrMaterial = mkU<LambertMaterial>(white_diffuse);

        primInsideOpaque1->mName = QString("Inside Opaque 1");
        primInsideOpaque1->mUPtrShape = mkU<Sphere>(Sphere());
        primInsideOpaque1->mUPtrShape->mTransform = transInsideOpaque1;
        scene.mPrimitiveList.push_back(std::move(primInsideOpaque1));


        // Reflective Sphere Blue1:
        uPtr<Primitive> primRefSphereBlue1 = mkU<Primitive>(Primitive());
        Transform transRefSphereBlue1 = Transform(Vector3f(-3, 3.5, -3), Vector3f(0, 0, 0), Vector3f(3, 3, 3));
        primRefSphereBlue1->mUPtrMaterial = mkU<LambertMaterial>(mirrored_specular2);

        primRefSphereBlue1->mName = QString("Reflective Sphere Blue1");
        primRefSphereBlue1->mUPtrShape = mkU<Sphere>(Sphere());
        primRefSphereBlue1->mUPtrShape->mTransform = transRefSphereBlue1;
        scene.mPrimitiveList.push_back(std::move(primRefSphereBlue1));


        // Transparent Sphere 3:
        uPtr<Primitive> primTransSphere3 = mkU<Primitive>(Primitive());
        Transform transTransSphere3 = Transform(Vector3f(3, 3.5, -3), Vector3f(0, 0, 0), Vector3f(3, 3, 3));
        primTransSphere3->mUPtrMaterial = mkU<LambertMaterial>(refractive_specular);

        primTransSphere3->mName = QString("Transparent Sphere 3");
        primTransSphere3->mUPtrShape = mkU<Sphere>(Sphere());
        primTransSphere3->mUPtrShape->mTransform = transTransSphere3;
        scene.mPrimitiveList.push_back(std::move(primTransSphere3));


        // Reflective Sphere Blue2:
        uPtr<Primitive> primRefSphereBlue2 = mkU<Primitive>(Primitive());
        Transform transRefSphereBlue2 = Transform(Vector3f(3, 3.5, -3), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primRefSphereBlue2->mUPtrMaterial = mkU<LambertMaterial>(mirrored_specular2);

        primRefSphereBlue2->mName = QString("Reflective Sphere Blue2");
        primRefSphereBlue2->mUPtrShape = mkU<Sphere>(Sphere());
        primRefSphereBlue2->mUPtrShape->mTransform = transRefSphereBlue2;
        scene.mPrimitiveList.push_back(std::move(primRefSphereBlue2));


        // Yellow Sphere:
        uPtr<Primitive> primYellowSphere = mkU<Primitive>(Primitive());
        Transform transYellowSphere = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(2, 2, 2));
        primYellowSphere->mUPtrMaterial = mkU<LambertMaterial>(yellow_diffuse);

        primYellowSphere->mName = QString("Yellow Sphere");
        primYellowSphere->mUPtrShape = mkU<Sphere>(Sphere());
        primYellowSphere->mUPtrShape->mTransform = transYellowSphere;
        scene.mPrimitiveList.push_back(std::move(primYellowSphere));
        */



        // Create sphere1 primitive:
        /*uPtr<Primitive> primSphere1 = mkU<Primitive>(Primitive());
        Transform sphereTransform1 = Transform(Vector3f(0, 0, 1), Vector3f(45, 0, 0), Vector3f(1, 1, 1));
        uPtr<Material> mMaterial1 = mkU<LambertMaterial>(LambertMaterial(Color3f(0, 1, 0)));
        primSphere1->mUPtrMaterial = std::move(mMaterial1);

        primSphere1->mName = QString("sphere");
        primSphere1->mUPtrShape = mkU<Sphere>(Sphere());
        primSphere1->mUPtrShape->mTransform = sphereTransform1;
        scene.mPrimitiveList.push_back(std::move(primSphere1));
        */
        // Create sphere2 primitive:
        /*uPtr<Primitive> primSphere2 = mkU<Primitive>(Primitive());
        Transform sphereTransform2 = Transform(Vector3f(-1, 0, 0), Vector3f(90, 0, 0), Vector3f(1, 1, 1));
        uPtr<Material> mMaterial2 = mkU<LambertMaterial>(LambertMaterial(Color3f(1, 0, 0)));
        primSphere2->mUPtrMaterial = std::move(mMaterial2);

        primSphere2->mName = QString("sphere");
        primSphere2->mUPtrShape = mkU<Sphere>(Sphere());
        primSphere2->mUPtrShape->mTransform = sphereTransform2;
        scene.mPrimitiveList.push_back(std::move(primSphere2));
        */
        // Create BlendSmooth primitive:
        /*uPtr<Primitive> primBlendSmooth = mkU<Primitive>(Primitive());
        primBlendSmooth->mName = QString("Blend Smooth");

        // uPtr<Material> mMaterial = mkU<LambertMaterial>(LambertMaterial(Color3f(0, 1, 0)));
        uPtr<Material> mMaterial = mkU<BlinnPhongMaterial>(BlinnPhongMaterial(Color3f(1, 1, 1), 5.f));
        primBlendSmooth->mUPtrMaterial = std::move(mMaterial);

        uPtr<SmoothBlend> mSmoothBlendPtr = mkU<SmoothBlend>(SmoothBlend());
        uPtr<Sphere> s1 = mkU<Sphere>(Sphere());
        uPtr<Sphere> s2 = mkU<Sphere>(Sphere());
        Transform sphereTransform1 = Transform(Vector3f(0.33, 0, 0), Vector3f(0, 0, 45), Vector3f(1, 1, 1));
        Transform sphereTransform2 = Transform(Vector3f(-0.33, 0, 0), Vector3f(90, 0, 0), Vector3f(1, 1, 1));
        s1->mTransform = sphereTransform1;
        s2->mTransform = sphereTransform2;
        mSmoothBlendPtr->mShape1 = s1.get();
        mSmoothBlendPtr->mShape2 = s2.get();
        mSmoothBlendPtr->mTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));
        primBlendSmooth->mUPtrShape = std::move(mSmoothBlendPtr);

        scene.mPrimitiveList.push_back(std::move(primBlendSmooth));*/

        // Create box primitive:
        /*uPtr<Primitive> primBox = mkU<Primitive>(Primitive());
        Transform boxTransform = Transform(Vector3f(0, 0, -2), Vector3f(0, 0, 0), Vector3f(3, 4.5, 1));
        uPtr<Material> materialBox = mkU<LambertMaterial>(yellow_diffuse);
        primBox->mUPtrMaterial = std::move(materialBox);

        primBox->mName = QString("box");
        primBox->mUPtrShape = mkU<Box>(Box());
        primBox->mUPtrShape->mTransform = boxTransform;
        scene.mPrimitiveList.push_back(std::move(primBox));*/




        // Create square plane primitive:
        /*uPtr<Primitive> primSquarePlane = mkU<Primitive>(Primitive());
        Transform squarePlaneTransform = Transform(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1.5, 1, 1));
        uPtr<Material> mMaterial3 = mkU<LambertMaterial>(LambertMaterial(Color3f(0, 1, 0)));
        primSquarePlane->mUPtrMaterial = std::move(mMaterial3);

        primSquarePlane->mName = QString("square");
        primSquarePlane->mUPtrShape = mkU<SquarePlane>(SquarePlane());
        primSquarePlane->mUPtrShape->mTransform = squarePlaneTransform;
        scene.mPrimitiveList.push_back(std::move(primSquarePlane));*/

        // Create capsule primitive:
        /*uPtr<Primitive> primCapsule = mkU<Primitive>(Primitive());
        Transform capsuleTransform = Transform(Vector3f(3, 2, -5), Vector3f(45, 0, 0), Vector3f(1, 1, 1));
        primCapsule->mName = QString("Capsule");
        primCapsule->mUPtrShape = mkU<Capsule>(Capsule());
        primCapsule->mUPtrShape->mTransform = capsuleTransform;
        scene.mPrimitiveList.push_back(std::move(primCapsule));*/



        // Rendering:
        for(int rowIdx = 0; rowIdx < 400; rowIdx++)
        {
            for(int colIdx = 0; colIdx < 400; colIdx++)
            {
                // Get a Ray.
                Ray pixel_ray = mainCamera.rayCast(rowIdx, colIdx);
                // Set Color.
                // Opt<Intersection> tempIntersection;
                QRgb value = qRgb(0, 0, 0);
                Color3f f_color = Color3f(0.f, 0.f, 0.f);

                // tempIntersection = scene.getIntersection(pixel_ray);
                // if(scene.getIntersection(pixel_ray, &tempIntersection))
                // if(tempIntersection != std::experimental::nullopt)
                if(scene.getColor(pixel_ray, f_color))
                {
                    // Hit an obj.
                    // glm::vec3 f_color = (tempIntersection->mNormal + glm::vec3(1, 1, 1)) * 0.5f * 255.f;
                    glm::ivec3 i_color = glm::ivec3(f_color);
                    value = qRgb(i_color.x, i_color.y, i_color.z);
                }

                ui->progressBar->setValue((rowIdx * 400 + colIdx) / (400 * 4));

                image.setPixel(colIdx, rowIdx, value);
            }
        }

        QString file_name = dir_name + "/RenderedResult" + QString(".png");

        image.save(file_name, "PNG");
    }
}
