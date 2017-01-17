//
//  ViewController.swift
//  salmontemplate
//
//  Created by Роберт Хайреев on 16/01/2017.
//
//

import UIKit
import GLKit

class ViewControllerTemplate: GLKViewController, UITextFieldDelegate {

    public var hiddenTextField: UITextField?
    private var context: EAGLContext?
    private var effect: GLKBaseEffect?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        guard let context = EAGLContext(api: .openGLES2) else  {
            print("Failed to create ES context")
            return
        }
        
        self.context = context
        let view: GLKView = self.view as! GLKView
        view.context = self.context!
        view.drawableDepthFormat = .format24
        
        //defaultView = view
        
        setupGL()
    
        let recognizer = UIPinchGestureRecognizer(target: self, action: #selector(respondToPinch(gestureRecognizer:)))
        recognizer.delaysTouchesEnded = false
        
        view.addGestureRecognizer(recognizer)
        view.isMultipleTouchEnabled = true
        
        hiddenTextField = UITextField(frame: CGRect(x: -200, y: -200, width: 0, height: 0))
        hiddenTextField?.autocorrectionType = .no
        view.addSubview(hiddenTextField!)
        hiddenTextField?.delegate = self
        NotificationCenter.default.addObserver(self, selector: #selector(onReceiveKeyboardNotification(notification:)), name: .UITextFieldTextDidChange, object: nil)
    }
    
    deinit {
        tearDownGL()
    }

    func setupGL() {
        EAGLContext.setCurrent(context)
        appInitCaller()
    }
    
    func tearDownGL() {
        EAGLContext.setCurrent(context)
        SE_AppDeinit()
    }
    
    func appInitCaller() {
       // CustomAppInit()
    }
    
    func respondToPinch(gestureRecognizer: UIPinchGestureRecognizer) {
        SE_AppOnScale(Float(gestureRecognizer.scale))
    }

    func onReceiveKeyboardNotification(notification: NSNotification) {
        if notification.name == .UITextFieldTextDidChange {
            let textField = notification.object as! UITextField
            let text = textField.text
            SE_SetKeyboardText(text)
        }
    }
    
    func textFieldDidBeginEditing(_ textField: UITextField) {
        print("Begin")
    }
    
    func textFieldDidEndEditing(_ textField: UITextField) {
        print("End")
    }
    
    func textField(_ textField: UITextField, shouldChangeCharactersIn range: NSRange, replacementString string: String) -> Bool {
        if string == "\n" {
            textField.resignFirstResponder()
            SE_OnKeyboardHide()
        }
        return true
    }
    
    func update() {
        SE_AppUpdate(Int32(self.timeSinceLastUpdate * 1000))
    }
    
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        SE_AppDraw()
    }

}


class ViewController: ViewControllerTemplate {
    override func appInitCaller() {
        CustomAppInit()
    }
    
    override var supportedInterfaceOrientations: UIInterfaceOrientationMask {
        return [.landscapeLeft, .landscapeRight]
    }
}
