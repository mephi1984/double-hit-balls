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
    var first: CGPoint!
    var second: Bool!
    var number: Int!
}



class GLKViewTemplate: GLKView {
    
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
            let location = touch.location(in: self)
            let prevLocation = touch.previousLocation(in: self)
            
            if abs(touchDict[touch]!.first!.x - location.x) > 10 || abs(touchDict[touch]!.first!.y - location.y) > 10 {
                touchDict[touch]?.second = true
            }
            
            let number = Int32(touchDict[touch]!.number)
            
            SE_AppOnScroll(Int32(Float(prevLocation.x) - Float(location.x)), -Int32(Float(prevLocation.y) - Float(location.y)), number)
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        for touch in touches {
            let location = touch.location(in: self)
            let number = Int32(touchDict[touch]!.number)
            
            if touchDict[touch]!.second == true {
                SE_AppOnTapUpAfterMove(Int32(location.x), Int32(bounds.size.height - location.y), number)
            } else {
                SE_AppOnTapUp(Int32(location.x), Int32(bounds.size.height - location.y), number)
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
