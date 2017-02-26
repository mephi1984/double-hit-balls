//
//  CustomGLKView.swift
//  salmontemplate
//
//  Created by Роберт Хайреев on 16/01/2017.
//
//

import Foundation
import GLKit

struct TTouchHashData {
    var first: CGPoint?
    var second: Bool?
    var number: Int?
}

class GLKViewTemplate: GLKView {
    
    let xCoef: CGFloat = 480 / UIScreen.main.bounds.size.width
    let yCoef: CGFloat = 320 / UIScreen.main.bounds.size.height

    var touchDict: [UITouch: TTouchHashData] = [:]
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        isMultipleTouchEnabled = true
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    func addTouchToHash(touch: UITouch) {
        let data = TTouchHashData()
        touchDict[touch] = data
        touchDict[touch]?.first = touch.location(in: self)
        touchDict[touch]?.second = false
        
        for n in 0...255 {
            var nExists = false
            
            for i in touchDict.values {
                if i.number == n {
                    nExists = true
                }
            }
            
            if !nExists {
                touchDict[touch]?.number = n
            }
            
        }
        
    }
    
    func removeTouchFromHash(touch: UITouch) {
        
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        for touch in touches {
            let location = CGPoint(x: touch.location(in: self).x * xCoef, y: touch.location(in: self).y * yCoef)
            addTouchToHash(touch: touch)
            
            guard let n = touchDict[touch]?.number else {
                return
            }
            
            let number = Int32(n)
            print(bounds.size.height * yCoef - location.y)
            SE_AppOnTapDown(Int32(location.x), Int32(bounds.size.height * yCoef - location.y), number)
        }
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        for touch in touches {
            let location = CGPoint(x: touch.location(in: self).x * xCoef, y: touch.location(in: self).y * yCoef)
            let prevLocation = CGPoint(x: touch.previousLocation(in: self).x * xCoef, y: touch.previousLocation(in: self).y * yCoef)
            
            guard let x = touchDict[touch]?.first?.x, let y = touchDict[touch]?.first?.y, let n = touchDict[touch]?.number else {
                return
            }
            if abs(x - location.x) > 10 || abs(y - location.y) > 10 {
                touchDict[touch]?.second = true
            }
            
            let number = Int32(n)
            
            SE_AppOnScroll(Int32(Float(prevLocation.x) - Float(location.x)), -Int32(Float(prevLocation.y) - Float(location.y)), number)
            //print("location is:\(location)!!!!")
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        for touch in touches {
            let location = CGPoint(x: touch.location(in: self).x * xCoef, y: touch.location(in: self).y * yCoef)
            //print("location is:\(location)!!!!")
            
            guard let n = touchDict[touch]?.number else {
                return
            }
            
            let number = Int32(n)
            
            if touchDict[touch]!.second == true {
                SE_AppOnTapUpAfterMove(Int32(location.x), Int32(bounds.size.height * yCoef - location.y), number)
            } else {
                SE_AppOnTapUp(Int32(location.x), Int32(bounds.size.height * yCoef - location.y), number)
            }
            touchDict.removeValue(forKey: touch)
        }
    }
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        for touch in touches {
            touchDict.removeValue(forKey: touch)
        }
    }
    
}

class CustomGLKView: GLKViewTemplate {
    required init?(coder aDecoder: NSCoder) {
        super.init(frame: UIScreen.main.bounds)
    }
}
