//
//  AppDelegate.swift
//  salmontemplate
//
//  Created by Роберт Хайреев on 16/01/2017.
//
//

import Foundation
import UIKit

@UIApplicationMain

class AppDelegate: UIResponder, UIApplicationDelegate {
    
    var window: UIWindow?
    var viewController: UIViewController?
    
    func applicationDidFinishLaunching(_ application: UIApplication) {
        window = UIWindow(frame: UIScreen.main.bounds)
        
        if UIDevice.current.userInterfaceIdiom == .phone {
            viewController = ViewController(nibName: "ViewController_iPhone", bundle: nil)
        } else {
            viewController = ViewController(nibName: "ViewController_iPad", bundle: nil)
        }
        
        window?.rootViewController = viewController
        window?.makeKeyAndVisible()
    }
    
}
