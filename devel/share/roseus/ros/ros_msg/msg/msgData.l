;; Auto-generated. Do not edit!


(when (boundp 'ros_msg::msgData)
  (if (not (find-package "ROS_MSG"))
    (make-package "ROS_MSG"))
  (shadow 'msgData (find-package "ROS_MSG")))
(unless (find-package "ROS_MSG::MSGDATA")
  (make-package "ROS_MSG::MSGDATA"))

(in-package "ROS")
;;//! \htmlinclude msgData.msg.html


(defclass ros_msg::msgData
  :super ros::object
  :slots (_Join_data _Join_state _data ))

(defmethod ros_msg::msgData
  (:init
   (&key
    ((:Join_data __Join_data) (make-array 6 :initial-element 0.0 :element-type :float))
    ((:Join_state __Join_state) (make-array 6 :initial-element 0.0 :element-type :float))
    ((:data __data) 0)
    )
   (send-super :init)
   (setq _Join_data __Join_data)
   (setq _Join_state __Join_state)
   (setq _data (round __data))
   self)
  (:Join_data
   (&optional __Join_data)
   (if __Join_data (setq _Join_data __Join_data)) _Join_data)
  (:Join_state
   (&optional __Join_state)
   (if __Join_state (setq _Join_state __Join_state)) _Join_state)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:serialization-length
   ()
   (+
    ;; float64[6] _Join_data
    (* 8    6)
    ;; float64[6] _Join_state
    (* 8    6)
    ;; uint32 _data
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64[6] _Join_data
     (dotimes (i 6)
       (sys::poke (elt _Join_data i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[6] _Join_state
     (dotimes (i 6)
       (sys::poke (elt _Join_state i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; uint32 _data
       (write-long _data s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64[6] _Join_data
   (dotimes (i (length _Join_data))
     (setf (elt _Join_data i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     )
   ;; float64[6] _Join_state
   (dotimes (i (length _Join_state))
     (setf (elt _Join_state i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     )
   ;; uint32 _data
     (setq _data (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get ros_msg::msgData :md5sum-) "73f187edf478e820d25171010c16a164")
(setf (get ros_msg::msgData :datatype-) "ros_msg/msgData")
(setf (get ros_msg::msgData :definition-)
      "float64[6] Join_data
float64[6] Join_state
uint32 data

")



(provide :ros_msg/msgData "73f187edf478e820d25171010c16a164")


